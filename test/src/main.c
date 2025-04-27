#include "test.h"

int main()
{
    char *malloc_ret = NULL;

    printf("%s", "TEST\n");
    malloc_ret = malloc(234);
    printf("%p\n", (void *)malloc_ret);
    return 0;
}