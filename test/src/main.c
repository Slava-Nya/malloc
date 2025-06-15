#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "test.h"

//#define assert(cond, msg) \
//    do { if (!(cond)) { \
//        fprintf(stderr, "❌ Assertion failed: %s\n", msg); \
//    } else { \
//        printf("✅ %s\n", msg); \
//    } } while (0)
//
//static void print_chunks(chunk_t* head) {
//    while (head) {
//        printf("  → chunk %p | size: %zu | free: %d | type: %d\n", (void*)head, head->size, head->free, head->type);
//        head = head->next;
//    }
//}
//
//static void print_zones(zone_type_t type) {
//    const char* name = (type == ZONE_TINY) ? "TINY" :
//                       (type == ZONE_SMALL) ? "SMALL" : "LARGE";
//    zone_t* zone = g_malloc[type].zones;
//    int zone_id = 0;
//
//    printf("\n--- %s zones ---\n", name);
//    while (zone) {
//        printf("Zone %d: %p\n", zone_id++, (void*)zone);
//        print_chunks(zone->head);
//        zone = zone->next;
//    }
//}

int main(void) {
    // TINY test
//    void* a = malloc(32);
//    assert(a != NULL, "malloc() tiny allocation not NULL");
//    strcpy((char*)a, "tiny test");
//    assert(strcmp((char*)a, "tiny test") == 0, "tiny chunk stores data correctly");
//
//    free(a);
//
//    // SMALL test
//    void* b = malloc(1024);
//    assert(b != NULL, "malloc() small allocation not NULL");
//    memset(b, 0xAA, 1024);
//    assert(((unsigned char*)b)[0] == 0xAA, "small chunk initialized and accessible");
//
//    free(b);
//
//    // LARGE test
//    size_t large_size = getpagesize() * 64;
//    void* c = malloc(large_size);
//    assert(c != NULL, "malloc() large allocation not NULL");
//    memset(c, 0xBB, large_size);
//    assert(((unsigned char*)c)[0] == 0xBB, "large chunk initialized and accessible");
//
//    free(c);
//
//    // Reuse test
//    void* d1 = malloc(32);
//    void* d2 = malloc(32);
//    free(d1);
//    void* d3 = malloc(32);
//    assert(d1 == d3, "free() chunk reused by malloc()");
//
//    free(d2);
//    free(d3);
//
//    // Print allocator state
//    print_zones(ZONE_TINY);
//    print_zones(ZONE_SMALL);
//    print_zones(ZONE_LARGE);

    test_main();

    printf("\nAll tests completed.\n");
    return 0;
}