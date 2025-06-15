#include "malloc.h"
#include "libft.h"


/**
 * Move tiny/small chunk to the end of its zone's list and mark as free.
 */
static void free_small_chunk(chunk_t* chunk) {
    zone_t* zone = chunk->zone;

    // Remove from current position
    if (chunk->prev)
        chunk->prev->next = chunk->next;
    else
        zone->head = chunk->next;

    if (chunk->next)
        chunk->next->prev = chunk->prev;
    else
        zone->tail = chunk->prev;

    // Append to tail
    chunk->free = true;
    chunk->prev = zone->tail;
    chunk->next = NULL;

    if (zone->tail)
        zone->tail->next = chunk;
    else
        zone->head = chunk;

    zone->tail = chunk;
}

static void free_large_chunk(chunk_t* chunk) {
    zone_t* zone = chunk->zone;

    // Unlink chunk from list
    if (chunk->prev)
        chunk->prev->next = chunk->next;
    else
        zone->head = chunk->next;

    if (chunk->next)
        chunk->next->prev = chunk->prev;

    // Unmap entire allocation (chunk + user data)
    munmap(chunk, sizeof(chunk_t) + chunk->size);
}

/**
 * Public free
 */
void free(void* ptr) {
    if (!ptr)
        return;

    chunk_t *chunk = (chunk_t *) ptr - 1;

    if (chunk->type == ZONE_LARGE)
        free_large_chunk(chunk);
    else
        free_small_chunk(chunk);
}