#ifndef MYMALLOC_H
#define MYMALLOC_H
#define MEMSIZE 4096
void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);
#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)
typedef struct header{
    int len;
    int isFree;
    struct header* next; // do we even use this lmao
}header;
#endif