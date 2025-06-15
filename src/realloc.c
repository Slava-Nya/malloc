#include "malloc.h"

void* realloc(void* ptr, size_t size) {
    chunk_t* chunk;

    if (!ptr)
        return malloc(size); // realloc(NULL, size) → malloc()

    if (size == 0) {
        free(ptr); // realloc(ptr, 0) → free()
        return NULL;
    }

    chunk = (chunk_t*)ptr - 1;
    // If new size fits in the same chunk, return as-is
    if ((chunk->type == ZONE_TINY && size <= TINY_CHUNK_SIZE) ||
        (chunk->type == ZONE_SMALL && size <= SMALL_CHUNK_SIZE)) {
        chunk->size = size;
        return ptr;
    }

    void* new_ptr = malloc(size);
    if (!new_ptr)
        return NULL;
    size_t copy_size = (size < chunk->size) ? size : chunk->size;
    memcpy(new_ptr, ptr, copy_size);
    free(ptr);

    return new_ptr;
}