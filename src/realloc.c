#include "malloc.h"

/**
 * Reallocate memory block to a new size.
 *
 * If the pointer is NULL, behaves like malloc(size).
 * If the size is zero, behaves like free(ptr) and returns NULL.
 * If the new size fits in the same chunk type (TINY or SMALL), the
 * same pointer is returned and the requested size is updated in the chunk
 * metadata for logging purposes.
 * Otherwise, a new allocation is made, the old data is copied,
 * and the old block is freed.
 */
void* realloc(void* ptr, size_t size) {
    if (!ptr && size > 0)
        return malloc(size);

    if (size == 0) {
        free(ptr); // realloc(ptr, 0) → free()
        return NULL;
    }

    chunk_t* chunk = find_valid_chunk(ptr);
    if (!chunk || chunk->free) {
        ft_putstr_fd("realloc(): invalid pointer\n", 2);
        abort();
    }

    // If new size fits in the same chunk, return as-is
    if ((chunk->type == ZONE_TINY && size <= TINY_CHUNK_SIZE) ||
        (chunk->type == ZONE_SMALL && size <= SMALL_CHUNK_SIZE)) {
        chunk->size = size;
        return ptr;
    }

    // Otherwise: allocate, copy, free
    void* new_ptr = malloc(size);
    if (!new_ptr)
        return NULL;

    size_t copy_size = (size < chunk->size) ? size : chunk->size;
    memcpy(new_ptr, ptr, copy_size);
    free(ptr);

    return new_ptr;
}