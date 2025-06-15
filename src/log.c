#include "malloc.h"
#include <stdio.h>

static const char* zone_names[] = {
        [ZONE_TINY] = "TINY",
        [ZONE_SMALL] = "SMALL",
        [ZONE_LARGE] = "LARGE"
};

void show_alloc_mem(void) {
    size_t total = 0;

    for (int type = 0; type <= ZONE_LARGE; type++) {
        zone_t* zone = g_malloc[type].zones;

        while (zone) {
            printf("%s : %p\n", zone_names[type], (void*)zone);
            chunk_t* chunk = zone->head;

            while (chunk) {
                if (!chunk->free) {
                    void* start = (void*)(chunk + 1);
                    void* end = (void*)((char*)start + chunk->size);
                    printf("%p - %p : %zu bytes\n", start, end, chunk->size);
                    total += chunk->size;
                }
                chunk = chunk->next;
            }

            zone = zone->next;
        }
    }

    printf("Total : %zu bytes\n", total);
}