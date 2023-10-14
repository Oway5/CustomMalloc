#ifndef MYMALLOC_H
#define MYMALLOC_H
void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);
#define mymalloc(s) mymalloc(s, __FILE__, __LINE__)
#define myfree(p) myfree(p, __FILE__, __LINE__)
#endif