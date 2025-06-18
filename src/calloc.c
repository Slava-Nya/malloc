#include "malloc.h"

/**
 *
 * Allocate zero-initialized memory for an array of elements.
 *
 * This function allocates memory for an array of `count` elements,
 * each of size `size` bytes, and sets the allocated memory to zero.
 * It performs an overflow check on the multiplication to avoid
 * undefined behavior for very large values.
 */
void* calloc(size_t count, size_t size) {
    size_t total_size = count * size;

    // Optional overflow check
    if (count != 0 && total_size / count != size)
        return NULL; // overflow detected

    void* ptr = malloc(total_size);
    if (!ptr)
        return NULL;

    ft_bzero(ptr, total_size);
    return ptr;
}