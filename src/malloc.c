
#include "malloc.h"
#include "libft.h"

void    *malloc(size_t size)
{
    printf("%zu fd\n", size);
    printf("TINY_HEAP_ALLOCATION_SIZE = %d\n", TINY_HEAP_ALLOCATION_SIZE);
    return NULL;
}

