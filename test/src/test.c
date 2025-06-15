#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "test.h"

#define M (1024 * 1024)

void print(const char *s) {
    write(1, s, strlen(s));
}

void test_0(void) {
    printf("\n[test_0] simple loop without allocation\n");
    int i = 0;
    while (i < 1024)
        i++;
    show_alloc_mem();
}

void test_1(void) {
    printf("\n[test_1] malloc without free\n");
    int i = 0;
    char *addr;
    while (i++ < 1024) {
        addr = (char *)malloc(1024);
        addr[0] = 42;
    }
    show_alloc_mem();
}

void test_2(void) {
    printf("\n[test_2] malloc and immediate free\n");
    int i = 0;
    char *addr;
    while (i++ < 1024) {
        addr = (char *)malloc(1024);
        addr[0] = 42;
        free(addr);
    }
    show_alloc_mem();
}

void test_3(void) {
    printf("\n[test_3] realloc large\n");
    char *addr1 = (char *)malloc(16 * M);
    strcpy(addr1, "Bonjours\n");
    print(addr1);
    char *addr3 = (char *)realloc(addr1, 128 * M);
    addr3[127 * M] = 42;
    print(addr3);
    show_alloc_mem();
}

void test_4(void) {
    printf("\n[test_4] realloc large with multiple malloc\n");
    char *addr1 = (char *)malloc(16 * M);
    strcpy(addr1, "Bonjours\n");
    print(addr1);
    char *addr2 = (char *)malloc(16 * M);
    (void)addr2;
    char *addr3 = (char *)realloc(addr1, 128 * M);
    addr3[127 * M] = 42;
    print(addr3);
    show_alloc_mem();
}

void test_5(void) {
    printf("\n[test_5] free with NULL and invalid pointer\n");
    char *addr = malloc(16);
    free(NULL);
    free((void *)addr + 5);
    if (realloc((void *)addr + 5, 10) == NULL)
        print("Bonjours\n");
    free(addr);
    show_alloc_mem();
}

void test_6(void) {
    printf("\n[test_6] realloc with 0 size and invalid pointer\n");
    char *addr = malloc(42);
    free(NULL);
    free((void *)addr + 5);
    if (realloc((void *)addr + 5, 0) == NULL)
        print("Bonjours\n");
    free(addr);
    show_alloc_mem();
}

void test_7(void) {
    printf("\n[test_7] multiple size class allocations\n");
    (void)malloc(1024);
    (void)malloc(1024 * 32);
    (void)malloc(1024 * 1024);
    (void)malloc(1024 * 1024 * 16);
    (void)malloc(1024 * 1024 * 128);
    show_alloc_mem();
}

void test_8(void) {
    printf("\n[test_2] malloc and immediate free\n");
    char *addr;
    addr = (char *)malloc(1);
    addr[0] = 42;
    show_alloc_mem();
    free(addr);
    show_alloc_mem();

}

void test_9(void) {
    printf("\n[test_2] malloc and immediate free\n");
    char *addr;
    addr = (char *)malloc(10);
    ft_strcpy(addr, "Lohan\n");
    printf("%s", addr);
    show_alloc_mem();
    free(addr);
    show_alloc_mem();

}

void test_main(void) {
//    test_0();
//    test_1();
//    test_2();
//    test_3();
//    test_4();
    test_5();
//    test_6();
//    test_7();
//    test_8();
//    test_9();
}
