#include "malloc.h"

/**
 * Validates and resolves a user pointer to its corresponding chunk.
 *
 * This function attempts to determine whether the given pointer points to a valid
 * user allocation managed by our custom malloc implementation. It works by converting
 * the user pointer to a chunk pointer (by subtracting 1) and then:
 *
 * - For TINY/SMALL chunks:
 *   - Ensures the chunk lies within the memory bounds of a known zone of the correct type.
 *   - Verifies that the chunk->zone pointer matches the zone being scanned.
 *
 * - For LARGE chunks:
 *   - Iterates the list of all LARGE allocations and matches the chunk pointer.
 */


struct zone *g_malloc[3];  // [ZONE_TINY], [ZONE_SMALL], [ZONE_LARGE]


chunk_t* find_valid_chunk(void* ptr) {
    if (!ptr)
        return NULL;

    // Scan TINY and SMALL zones first
    for (zone_type_t type = ZONE_TINY; type <= ZONE_SMALL; type++) {
        zone_t* zone = g_malloc[type];

        while (zone) {
            char* zone_start = (char*)zone;
            char* zone_end = zone_start + ZONE_SIZE(type);
            char* chunk_ptr = (char*)ptr - sizeof(chunk_t);

            // Check if chunk lies within this zone
            if (chunk_ptr >= zone_start && chunk_ptr < zone_end) {
                chunk_t* chunk = (chunk_t*)chunk_ptr;
                if (chunk->zone == zone && chunk->type == type)
                    return chunk;
            }

            zone = zone->next;
        }
    }

    // Then scan large zones (no size boundary guarantees)
    zone_t* zone = g_malloc[ZONE_LARGE];
    if (!zone)
        return NULL;
    chunk_t* chunk = zone->head;
    while (chunk) {
        if ((void*)(chunk + 1) == ptr)
            return chunk;
        chunk = chunk->next;
    }

    return NULL;
}