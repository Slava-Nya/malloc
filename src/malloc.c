
#include "malloc.h"
#include "libft.h"

// External global malloc state
malloc_state_t g_malloc = {0};

static void initialize_zone_chunks(zone_t* zone, size_t zone_size, size_t chunk_size) {
    // Point to the first chunk, right after the zone header
    char* base = (char*)zone + sizeof(zone_t);
    char* end  = (char*)zone + zone_size;
    chunk_t* prev = NULL;

    while (base + sizeof(chunk_t) + chunk_size <= end) {
        chunk_t* chunk = (chunk_t*)base;
        chunk->free = true;
        chunk->prev = prev;
        chunk->next = NULL;

        if (prev)
            prev->next = chunk;
        else
            zone->head = chunk;

        prev = chunk;
        base += sizeof(chunk_t) + chunk_size;
    }

    zone->tail = prev;
}

/**
 * Allocate a new zone using mmap.
 * Returns a pointer to the new zone or NULL on failure.
 */
static zone_t* create_zone(zone_type_t zone_type, size_t zone_size, size_t chunk_size)
{
    void* ptr = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (ptr == MAP_FAILED)
        return NULL;

    zone_t* zone = (zone_t*)ptr;
    ft_bzero(zone, sizeof(zone));
    zone->type = zone_type;
    pthread_mutex_init(&zone->lock, NULL);

    initialize_zone_chunks(zone, zone_size, chunk_size);

    return zone;
}

static chunk_t* get_free_chunk(zone_t* zone)
{
    if (!zone || !zone->tail || !zone->tail->free)
        return NULL; // No free chunk

    // Pick the last free chunk (tail)
    chunk_t* chunk = zone->tail;

    // Remove from tail
    zone->tail = chunk->prev;
    if (zone->tail)
        zone->tail->next = NULL;
    else
        zone->head = NULL;

    // Insert at head (mark as used)
    chunk->prev = NULL;
    chunk->next = zone->head;
    if (zone->head)
        zone->head->prev = chunk;
    zone->head = chunk;

    chunk->free = false;

    return chunk;
}

/**
 * Handle large allocations by using mmap directly.
 */
static void* large_alloc(size_t size) {
    size_t total_size = sizeof(chunk_t) + size;

    void* ptr = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (ptr == MAP_FAILED)
        return NULL;

    chunk_t* chunk = (chunk_t*)ptr;
    ft_bzero(chunk, sizeof(chunk_t));

    // Insert into the large zone list
    chunk->next = (chunk_t*)g_malloc.large_zones;
    g_malloc.large_zones = (zone_t*)chunk;

    return (void*)(chunk + 1);
}

static void* _malloc(zone_t** zone_list, zone_t zone_type, size_t zone_size, size_t chunk_size)
{
    chunk_t* chunk;

    chunk = get_free_chunk(*zone_list);
    if (!chunk)
    {
        zone_t *new_zone = create_zone(zone_type, zone_size, chunk_size);
        if (!new_zone)
            return NULL;
        new_zone->next = *zone_list;
        *zone_list = new_zone;
        chunk = get_free_chunk(*zone_list);
    }

    return (void*)(chunk + 1);
}

void* malloc(size_t size) {
    if (size == 0)
        return NULL;

    chunk_t* chunk = NULL;
    if (size <= TINY_CHUNK_SIZE)
        chunk = _malloc(&g_malloc.tiny_zones, ZONE_TINY, TINY_ZONE_ALLOCATION_SIZE, TINY_CHUNK_SIZE);
    else if (size <= SMALL_CHUNK_SIZE)
        chunk = _malloc(&g_malloc.small_zones, ZONE_SMALL, SMALL_ZONE_ALLOCATION_SIZE, SMALL_CHUNK_SIZE);
    else
        return large_alloc(size);

    if (!chunk)
        return NULL;

    return (void*)(chunk + 1);
}
