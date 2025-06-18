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


/*
** For a pagesize of 16384 bytes
**
** TINY  - chunk < 128 bytes  - zone 64 KB
** SMALL - chunk < 1024 bytes - zone 512 KB
** LARGE - chunk > 1024 bytes
*/
# define TINY_ZONE_ALLOCATION_SIZE ((size_t)(4 * getpagesize()))
# define TINY_CHUNK_SIZE (TINY_ZONE_ALLOCATION_SIZE / 128)

# define SMALL_ZONE_ALLOCATION_SIZE ((size_t)(32 * getpagesize()))
# define SMALL_CHUNK_SIZE (SMALL_ZONE_ALLOCATION_SIZE / 128)

# define IS_TINY(size)   ((size) <= (size_t)TINY_CHUNK_SIZE)
# define IS_SMALL(size)  ((size) <= (size_t)SMALL_CHUNK_SIZE && (size) > (size_t)TINY_CHUNK_SIZE)

#define ZONE_SIZE(type) ((type) == ZONE_TINY ? TINY_ZONE_ALLOCATION_SIZE : SMALL_ZONE_ALLOCATION_SIZE)
#define CHUNK_SIZE(type) ((type) == ZONE_TINY ? TINY_CHUNK_SIZE : SMALL_CHUNK_SIZE)

typedef enum e_zone_type {
    ZONE_TINY = 0,
    ZONE_SMALL = 1,
    ZONE_LARGE = 2
}           zone_type_t;

// Chunk inside a zone or standalone for large
typedef struct chunk {
    struct chunk*    next;   // next chunk in the list
    struct chunk*    prev;   // previous chunk in the list
    struct zone*     zone;    // link back to parent zone
    bool             free;   // only meaningful for tiny/small
    zone_type_t      type;   // ZONE_TINY / ZONE_SMALL / ZONE_LARGE
    size_t           size;   // only used for large (user-requested size)
} chunk_t;


// Zone of memory, containing fixed-size chunks (tiny/small)
typedef struct zone {
    struct zone*     next;   // next zone of the same type
    chunk_t*         head;   // first chunk in the list
    chunk_t*         tail;   // last chunk in the list
    pthread_mutex_t  lock;   // for thread-safe operations
} zone_t;

// Global allocator state: two classes, accessed as g_malloc
typedef struct malloc_state {
    zone_t* zones;
} malloc_state_t;

malloc_state_t g_malloc[3];  // [ZONE_TINY], [ZONE_SMALL], [ZONE_LARGE]


// Malloc API
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void *calloc(size_t count, size_t size);
void free(void *ptr);

void show_alloc_mem(void);
chunk_t* find_valid_chunk(void* ptr);

#endif