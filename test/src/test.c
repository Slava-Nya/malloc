#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>
#include "test.h"

#define M (1024 * 1024)

void print(const char *s) {
    write(1, s, strlen(s));
}

static sigjmp_buf jump_buffer;

void handler(int sig) {
    (void)sig;
    write(1, "[ABORTED] Test triggered abort\n", 32);
    siglongjmp(jump_buffer, 1);
}

void run_test(const char *name, void (*func)(void), int expect_abort) {
    printf("\n[RUNNING] %s\n", name);
    if (expect_abort) {
        signal(SIGABRT, handler);
        if (sigsetjmp(jump_buffer, 1) == 0) {
            func();
            printf("[FAILED] Expected abort but it returned normally\n");
        } else {
            printf("[PASSED] %s\n", name);
        }
    } else {
        func();
        printf("[PASSED] %s\n", name);
    }
}

void test_simple_loop(void) {
    for (int i = 0; i < 1024; i++);
}

void test_malloc_no_free(void) {
    for (int i = 0; i < 1024; i++) {
        char *addr = malloc(1024);
        addr[0] = 42;
    }
}

void test_malloc_with_free(void) {
    for (int i = 0; i < 1024; i++) {
        char *addr = malloc(1024);
        addr[0] = 42;
        free(addr);
    }
}

void test_realloc_large(void) {
    char *addr = malloc(16 * M);
    strcpy(addr, "Bonjours\n");
    print(addr);
    addr = realloc(addr, 128 * M);
    addr[127 * M] = 42;
    print(addr);
}

void test_realloc_with_other_allocs(void) {
    char *addr1 = malloc(16 * M);
    strcpy(addr1, "Bonjours\n");
    print(addr1);
    (void)malloc(16 * M);
    addr1 = realloc(addr1, 128 * M);
    addr1[127 * M] = 42;
    print(addr1);
}

void test_calloc(void) {
    char *addr = calloc(10, sizeof(char));
    int success = 1;
    for (int i = 0; i < 10; i++) {
        if (addr[i] != 0) {
            success = 0;
            break;
        }
    }
    if (success) printf("calloc passed\n");
    else printf("calloc failed\n");
    free(addr);
}

void test_invalid_free(void) {
    char *addr = malloc(42);
    free(NULL);
    free((void *)addr + 5); // should abort
    free(addr);
}

void test_realloc_invalid(void) {
    char *addr = malloc(42);
    if (realloc((void *)addr + 5, 0) == NULL)
        print("Realloc invalid pointer returned NULL as expected\n");
    free(addr);
}

void test_size_classes(void) {
    (void)malloc(1024);             // TINY
    (void)malloc(1024 * 32);        // SMALL
    (void)malloc(1024 * 1024);      // LARGE
    (void)malloc(1024 * 1024 * 16); // LARGE
    (void)malloc(1024 * 1024 * 128);// LARGE
}

void test_single_alloc_free(void) {
    char *addr = malloc(1);
    addr[0] = 42;
    show_alloc_mem();
    free(addr);
    show_alloc_mem();
}

void test_print_string(void) {
    char *addr = malloc(10);
    strcpy(addr, "Lohan\n");
    printf("%s", addr);
    show_alloc_mem();
    free(addr);
}

int main(void) {
    run_test("test_single_alloc_free", test_single_alloc_free, 0);
    run_test("test_print_string", test_print_string, 0);
    run_test("test_simple_loop", test_simple_loop, 0);
    run_test("test_malloc_no_free", test_malloc_no_free, 0);
    run_test("test_malloc_with_free", test_malloc_with_free, 0);
    run_test("test_realloc_large", test_realloc_large, 0);
    run_test("test_realloc_with_other_allocs", test_realloc_with_other_allocs, 0);
    run_test("test_calloc", test_calloc, 0);
    run_test("test_invalid_free", test_invalid_free, 1);
    run_test("test_realloc_invalid", test_realloc_invalid, 0);
    run_test("test_size_classes", test_size_classes, 0);

}