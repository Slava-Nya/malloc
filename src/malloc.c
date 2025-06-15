#include "malloc.h"
#include "libft.h"

/* Initialize all chunks inside the zone */
static void initialize_zone_chunks(zone_t* zone, size_t zone_size, size_t chunk_size, zone_type_t zone_type) {
    size_t header_size = sizeof(zone_t);
    size_t chunk_total = sizeof(chunk_t) + chunk_size;
    size_t max_chunks = (zone_size - header_size) / chunk_total;

    char* base = (char*)zone + header_size;
    chunk_t* prev = NULL;

    for (size_t i = 0; i < max_chunks; i++) {
        chunk_t* chunk = (chunk_t*)(base + i * chunk_total);
        chunk->free = true;
        chunk->type = zone_type;
        chunk->size = chunk_size;
        chunk->prev = prev;
        chunk->next = NULL;
        chunk->zone = zone;

        if (prev)
            prev->next = chunk;
        else
            zone->head = chunk;

        prev = chunk;
    }

    zone->tail = prev;
}

/**
 * Search through all zones of the given type for a free chunk.
 * If found, move the chunk to the head (mark as used) and return it.
 */
static chunk_t* get_free_chunk(zone_t* zone_list) {
    zone_t* zone = zone_list;

    while (zone) {
        if (zone->tail && zone->tail->free) {
            chunk_t* chunk = zone->tail;

            // Remove chunk from the end (tail)
            zone->tail = chunk->prev;
            if (zone->tail)
                zone->tail->next = NULL;
            else
                zone->head = NULL;

            // Insert chunk at the front (head), mark as used
            chunk->prev = NULL;
            chunk->next = zone->head;
            if (zone->head)
                zone->head->prev = chunk;
            zone->head = chunk;

            chunk->free = false;
            return chunk;
        }
        zone = zone->next; // Go to the next zone
    }

    return NULL; // No free chunk available
}

/* Allocate a new zone using mmap */
static zone_t* create_zone(size_t zone_size) {
    void* ptr = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (ptr == MAP_FAILED)
        return NULL;

    zone_t* zone = (zone_t*)ptr;
    ft_bzero(zone, sizeof(zone_t));
    return zone;
}

/* Handle large allocations with mmap and track in large zone */
static chunk_t * large_alloc(size_t size) {
    size_t total_size = sizeof(chunk_t) + size;
    void* ptr = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (ptr == MAP_FAILED)
        return NULL;

    chunk_t* chunk = (chunk_t*)ptr;
    ft_bzero(chunk, sizeof(chunk_t));
    chunk->type = ZONE_LARGE;
    chunk->size = size;

    zone_t* zone = g_malloc[ZONE_LARGE].zones;
    if (!zone) {
        zone = create_zone(sizeof(zone_t));
        zone->head = zone->tail = chunk;
        g_malloc[ZONE_LARGE].zones = zone;
    } else {
        chunk->next = zone->head;
        if (zone->head)
            zone->head->prev = chunk;
        zone->head = chunk;
    }

    chunk->zone = zone;
    return chunk;
}

/* Internal malloc logic for tiny and small */
static chunk_t * alloc(zone_type_t zone_type, size_t zone_size, size_t chunk_size) {
    chunk_t* chunk = NULL;
    zone_t** zone_list = &g_malloc[zone_type].zones;

    chunk = get_free_chunk(*zone_list);
    if (!chunk) {
        zone_t* new_zone = create_zone(zone_size);
        initialize_zone_chunks(new_zone, zone_size, chunk_size, zone_type);
        new_zone->next = *zone_list;
        *zone_list = new_zone;
        chunk = get_free_chunk(new_zone);
    }

    return chunk;

}

/* Public malloc */
void* malloc(size_t size) {
    chunk_t *chunk = NULL;

    if (size == 0)
        return NULL;

    if (IS_TINY(size))
        chunk = alloc(ZONE_TINY, TINY_ZONE_ALLOCATION_SIZE, TINY_CHUNK_SIZE);
    else if (IS_SMALL(size))
        chunk = alloc(ZONE_SMALL, SMALL_ZONE_ALLOCATION_SIZE, SMALL_CHUNK_SIZE);
    else
        chunk = large_alloc(size);
    if (!chunk)
        return NULL;
    chunk->size = size;

    return (void*)(chunk + 1);
}