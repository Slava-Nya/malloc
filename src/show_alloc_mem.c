#include "malloc.h"

/**
 * Show current memory allocations by zone type.
 *
 * This function walks through all memory zones in g_malloc,
 * and prints each zone's address followed by the address ranges
 * of allocated (non-free) chunks and their sizes. Only used memory
 * is shown, free chunks are skipped.
 *
 * Output is printed to stdout using write-based ft_* functions.
 * The total number of allocated bytes is printed at the end.
 */
void show_alloc_mem(void) {
    static const char *zone_names[] = { "TINY", "SMALL", "LARGE" };
    size_t total = 0;

    for (int type = 0; type <= ZONE_LARGE; type++) {
        zone_t* zone = g_malloc[type].zones;

        while (zone) {
            ft_putstr_fd((char *)zone_names[type], 1);
            ft_putstr_fd(" : 0x", 1);
            ft_puthex_fd((size_t)zone, 1);
            ft_putchar_fd('\n', 1);

            chunk_t* chunk = zone->head;
            while (chunk) {
                if (!chunk->free) {
                    void* start = (void*)(chunk + 1);
                    void* end = (void*)((char*)start + chunk->size);

                    ft_putstr_fd("  0x", 1);
                    ft_puthex_fd((size_t)start, 1);
                    ft_putstr_fd(" - 0x", 1);
                    ft_puthex_fd((size_t)end, 1);
                    ft_putstr_fd(" : ", 1);
                    ft_putnbr_fd(chunk->size, 1);
                    ft_putstr_fd(" bytes\n", 1);

                    total += chunk->size;
                }
                chunk = chunk->next;
            }

            zone = zone->next;
        }
    }

    ft_putstr_fd("Total : ", 1);
    ft_putnbr_fd(total, 1);
    ft_putstr_fd(" bytes\n", 1);
}