#ifndef MALLOC_H
# define MALLOC_H

#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>

#include "libft.h"

typedef enum e_zone_type {
    ZONE_TINY,
    ZONE_SMALL,
    ZONE_LARGE
} zone_type_t;



void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void *calloc(size_t count, size_t size);
void free(void *ptr);

#endif