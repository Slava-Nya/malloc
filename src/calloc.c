#include "malloc.h"

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