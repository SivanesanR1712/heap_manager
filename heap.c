#include <stdio.h>
#include <stdint.h>
#include"heap.h"
#include <stdalign.h>
#define HEAP_SIZE 1024
#define MIN_PAYLOAD 8
#define ALIGN MIN_PAYLOAD

alignas(8) unsigned char heap[HEAP_SIZE];


header *heap_ptr = NULL;

void init() {
    heap_ptr = (header*)heap;
    heap_ptr->size = HEAP_SIZE - sizeof(header);
    heap_ptr->is_free = 1;
    heap_ptr->nxt_ptr = NULL;
}

void* minimalloc(int bytes) {
    if (bytes <= 0) 
     return NULL;

   // check if bytes are 8 bytes aligned 
    if (bytes % ALIGN != 0)
     bytes += ALIGN - (bytes % ALIGN);

    header *ptr = heap_ptr;

    while (ptr != NULL) {
        if (ptr->is_free && ptr->size >= bytes)
        {
            int old_size = ptr->size;

           
            if (old_size >= bytes + (int)sizeof(header) + MIN_PAYLOAD)// check if the remaining size is greater than the minimium allocatable size
            {
            
                header *new_free = (header*)((char*)ptr + sizeof(header) + bytes);
                new_free->size = old_size - bytes - sizeof(header);
                new_free->is_free = 1;
                new_free->nxt_ptr = ptr->nxt_ptr;

                ptr->nxt_ptr = new_free;
                ptr->size = bytes;
            }

            ptr->is_free = 0;
            return (void*)((char*)ptr + sizeof(header));
        }
        ptr = ptr->nxt_ptr;
    }
    return NULL; 
}
static int ptr_in_heap(const void *p)
{
    const uintptr_t start = (uintptr_t)heap;
    const uintptr_t end   = (uintptr_t)heap + HEAP_SIZE;
    const uintptr_t temp     = (uintptr_t)p;

    return (temp >= start + sizeof(header)) && (temp < end);
}

static int is_aligned_8(const void *p)
{
    return (((uintptr_t)p) & (ALIGN-1)) == 0;
}

void minifree(void *ptr) {
    if (ptr == NULL) // invalid pointer; 
     return;
    
    if (!ptr_in_heap(ptr) || !is_aligned_8(ptr))        
     return; // invalid pointer    
        
    header *block = (header*)((char*)ptr - sizeof(header));
    block->is_free = 1;
    
    if (block->nxt_ptr && block->nxt_ptr->is_free) 
    {
        block->size += block->nxt_ptr->size + sizeof(header);
        block->nxt_ptr = block->nxt_ptr->nxt_ptr;
    }
    
    header *ptr1 = heap_ptr;
    while (ptr1&& ptr1->nxt_ptr) 
    {
        if (ptr1->is_free && ptr1->nxt_ptr->is_free)
        {
            ptr1->size += ptr1->nxt_ptr->size + sizeof(header);
            ptr1->nxt_ptr = ptr1->nxt_ptr->nxt_ptr;
        } else {
            ptr1 = ptr1->nxt_ptr;
        }
    }
}

void heap_dump(void)
{
    header *p = heap_ptr;
    int idx = 0;

    printf("\n========== HEAP DUMP ==========\n");
    printf("heap base=%p  heap end=%p  header=%zu bytes\n",
           (void*)heap, (void*)(heap + HEAP_SIZE), sizeof(header));

    while (p) {
        void *payload = (void *)((unsigned char*)p + sizeof(header));
        printf("#%02d  Hdr=%p  Pay=%p  size=%4d  %s  next=%p\n",
               idx,
               (void*)p,
               payload,
               p->size,
               p->is_free ? "FREE" : "USED",
               (void*)p->nxt_ptr);

        p = p->nxt_ptr;
        idx++;
    }

    printf("================================\n\n");
}

