/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlorrine <hlorrine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 23:51:34 by hlorrine          #+#    #+#             */
/*   Updated: 2025/06/19 18:50:41 by hlorrine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/**
 * Initialize all chunks inside the zone.
 *
 * This function splits the allocated memory region (zone) into
 * fixed-size chunks of `chunk_t + CHUNK_SIZE(type)` size and
 * links them into a doubly-linked list.
 */
static void initialize_zone_chunks(zone_t* zone, zone_type_t zone_type) {
    size_t header_size = sizeof(zone_t);
    size_t chunk_total = sizeof(chunk_t) + CHUNK_SIZE(zone_type);
    size_t max_chunks = (ZONE_SIZE(zone_type) - header_size) / chunk_total;

    char* base = (char*)zone + header_size;
    chunk_t* prev = NULL;

    for (size_t i = 1; i < max_chunks; i++) {
        chunk_t* chunk = (chunk_t*)(base + i * chunk_total);
        chunk->free = true;
        chunk->type = zone_type;
        chunk->size = 0;
        chunk->prev = prev;
        chunk->next = NULL;
        chunk->zone = zone;

        if (prev)
            prev->next = chunk;
        else
            zone->head = chunk;

        prev = chunk;
    }

    zone->tail = prev;
}

/**
 * Search through all zones of the given type for a free chunk.
 *
 * Returns the last free chunk in the most recently used zone
 * and moves it to the head of the list to mark it as used.
 */
static chunk_t* get_free_chunk(zone_t* zone_list) {
    zone_t* zone = zone_list;

    while (zone) {
        if (zone->tail && zone->tail->free) {
            chunk_t* chunk = zone->tail;

            zone->tail = chunk->prev;
            if (zone->tail)
                zone->tail->next = NULL;
            else
                zone->head = NULL;

            chunk->prev = NULL;
            chunk->next = zone->head;
            if (zone->head)
                zone->head->prev = chunk;
            zone->head = chunk;

            chunk->free = false;
            return chunk;
        }
        zone = zone->next;
    }

    return NULL;
}

/**
 * Allocate a new zone using mmap.
 *
 * The returned memory will contain zone metadata followed by
 * space for fixed-size chunks. Returns NULL on failure.
 */
static zone_t* create_zone(size_t zone_size) {
    void* ptr = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (ptr == MAP_FAILED) {
        ft_putstr_fd("malloc(): mmap failed\n", 2);
        abort();
    }

    zone_t* zone = (zone_t*)ptr;
    ft_bzero(zone, sizeof(zone_t));
    return zone;
}

/**
 * Handle large allocations via mmap and insert into large zone list.
 *
 * Each large allocation gets its own mapping and chunk metadata.
 * It is inserted at the head of the linked list in the large zone.
 */
static chunk_t* large_alloc(size_t size) {
    size_t total_size = sizeof(chunk_t) + size;
    void* ptr = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (ptr == MAP_FAILED) {
        ft_putstr_fd("malloc(): mmap failed\n", 2);
        abort();
    }

    chunk_t* chunk = (chunk_t*)ptr;
    ft_bzero(chunk, sizeof(chunk_t));
    chunk->type = ZONE_LARGE;
    chunk->size = size;

    zone_t* zone = g_malloc[ZONE_LARGE].zones;
    if (!zone) {
        zone = create_zone(sizeof(zone_t));
        zone->head = zone->tail = chunk;
        g_malloc[ZONE_LARGE].zones = zone;
    } else {
        chunk->next = zone->head;
        if (zone->head)
            zone->head->prev = chunk;
        zone->head = chunk;
    }

    chunk->zone = zone;
    return chunk;
}

/**
 * Internal allocator for TINY and SMALL zones.
 *
 * Attempts to get a free chunk. If no chunks are available,
 * creates a new zone and retries.
 */
static chunk_t* alloc(zone_type_t zone_type) {
    zone_t** zone_list = &g_malloc[zone_type].zones;

    chunk_t* chunk = get_free_chunk(*zone_list);
    if (!chunk) {
        zone_t* new_zone = create_zone(ZONE_SIZE(zone_type));
        initialize_zone_chunks(new_zone, zone_type);
        new_zone->next = *zone_list;
        *zone_list = new_zone;
        chunk = get_free_chunk(new_zone);
    }

    return chunk;
}

/**
 * Public malloc
 *
 * Returns a user-usable pointer to an allocated chunk.
 * Chooses zone type based on size and either reuses a chunk
 * or creates a new one if necessary.
 */
void* malloc(size_t size) {
    if (size == 0)
        return NULL;

    chunk_t* chunk = NULL;

    if (IS_TINY(size))
        chunk = alloc(ZONE_TINY);
    else if (IS_SMALL(size))
        chunk = alloc(ZONE_SMALL);
    else
        chunk = large_alloc(size);

    if (!chunk)
        return NULL;

    chunk->size = size;
    return (void*)(chunk + 1);
}