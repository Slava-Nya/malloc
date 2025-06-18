#include "malloc.h"
#include "libft.h"


/**
 * dsMove tiny/small chunk to the end of its zone's list and mark as free.
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

/**
 * Free a large chunk by unlinking it from the zone's list
 * and unmapping the entire memory region allocated via mmap.
 */
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
 *
 * Frees a previously allocated memory block.
 * Determines the chunk type from metadata and dispatches
 * to the appropriate free routine (small/tiny or large).
 *
 * If the pointer is NULL, the function does nothing.
 */
void free(void* ptr) {
    chunk_t *chunk = find_valid_chunk(ptr);
    if (!chunk) {
        ft_putstr_fd("free(): invalid pointer\n", 2);
        abort();
    }
    if (chunk->free) {
        ft_putstr_fd("free(): double free detected\n", 2);
        abort();
    }

    if (chunk->type == ZONE_LARGE)
        free_large_chunk(chunk);
    else
        free_small_chunk(chunk);
}
