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
** TINY  - block < 128 bytes  - heap 64 KB
** SMALL - block < 1024 bytes - heap 512 KB
** LARGE - block > 1024 bytes
*/

# define TINY_ZONE_ALLOCATION_SIZE (4 * getpagesize())
# define TINY_CHUNK_SIZE (TINY_HEAP_ALLOCATION_SIZE / 128)
# define SMALL_ZONE_ALLOCATION_SIZE (32 * getpagesize())
# define SMALL_CHUNK_SIZE (SMALL_HEAP_ALLOCATION_SIZE / 128)


typedef enum e_zone_type {
    ZONE_TINY,
    ZONE_SMALL,
    ZONE_LARGE
}           zone_type_t;


// Chunk inside a zone
typedef struct chunk {
    struct chunk* next;      // next chunk in the list
    struct chunk* prev;      // previous chunk in the list
    bool free;                // 1 if free, 0 if used
} chunk_t;


// Zone of memory, containing chunks
typedef struct zone {
    struct zone* next;       // next zone of the same type
    struct chunk* head;      // head of the chunk list
    struct chunk* tail;      // tail of the chunk list
    int type;                // type of the zone (TINY, SMALL)
    pthread_mutex_t lock;    // mutex for thread safety
} zone_t;


// Global allocator state
typedef struct malloc_state {
    zone_t* tiny_zones;       // list of TINY zones
    zone_t* small_zones;      // list of SMALL zones
    zone_t* large_zones;      // list of LARGE allocations
    pthread_mutex_t global_lock; // global lock for malloc state (optional)
} malloc_state_t;


// External global malloc state
extern malloc_state_t g_malloc;


// Malloc API
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void *calloc(size_t count, size_t size);
void free(void *ptr);

static void initialize_zone_chunks(zone_t* zone, size_t zone_size, size_t chunk_size);
static zone_t* create_zone(int type, size_t zone_size, size_t chunk_size);

#endif