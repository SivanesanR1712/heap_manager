typedef struct header {
    int size;
    int is_free;
    struct header *nxt_ptr;
} header;

void init();
void* minimalloc(int bytes);
void minifree(void *ptr) ;
void heap_dump(void);


