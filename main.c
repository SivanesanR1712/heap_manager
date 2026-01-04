#include<stdio.h>
#include"heap.h"

int main(void)
{
    init();
    heap_dump();

    int *a = minimalloc(16);
    heap_dump();

    int *b = minimalloc(32);
    heap_dump();

    minifree(a);
    heap_dump();

    minifree(b);
    heap_dump();

    return 0;
}

