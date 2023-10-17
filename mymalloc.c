#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <stdbool.h>

//#define MEMSIZE 4096  //not sure if this is the size we want. we can also make a macro from the .h and make it a static var here
//moved memsize to .h cause I'm still not sure what we're supposed to do with it and I'm too lazy to read the instructions so we gonna make it a macro in the .h
#define ROUNDUP8(x) ( ( (x) + 7 ) & (-7))

static char memory[MEMSIZE];
void* memoryStart = (void*)memory;  // Start of  memory block
void* memoryEnd = (void*)(memory + MEMSIZE); // end of memory block



void myfree(void *ptr, char *file, int line){
    if (!ptr) {
        printf("ERROR: Attempt to free NULL pointer in file %s at line %d\n", file, line);
        return;
    }
    if (ptr < (void*)memory || ptr >= (void*)(memory + MEMSIZE)) {
        printf("ERROR: Pointer out of bounds in file %s at line %d\n", file, line);
        return;
    }

    header *current = (header *)memory;
    header *prev = NULL;
    header *toFree = (header *)((char *)ptr - sizeof(header));

    // Validate the pointer and find its header in the memory block
    while (current != NULL && (char *)current < memory + MEMSIZE) {
        if (current == toFree) {
            break;
        }
        prev = current;
        current = (header *)((char *)current + sizeof(header) + current->len);
    }

    // Check if the pointer was not found or is already freed
    if (current != toFree || current->isFree) {
        printf("ERROR: Invalid free operation in file %s at line %d\n", file, line);
        return;
    }

    // Mark the chunk as free
    current->isFree = 1;

    // Coalesce with next chunk if it's free
    header *next = (header *)((char *)current + sizeof(header) + current->len);
    if ((char *)next < memory + MEMSIZE && next->isFree) {
        current->len += sizeof(header) + next->len;
        current->next = next->next;
    }

    // Coalesce with previous chunk if it's free
    if (prev != NULL && prev->isFree) {
        prev->len += sizeof(header) + current->len;
        prev->next = current->next;
    }
}




int GetChunkSize(header* h) {
    return h->len;
}

// Checks if the chunk is free
int IsFree(header* h) {
    return h->isFree;
}

// Sets the size of the chunk in the header
void SetChunkSize(header* h, int size) {
    h->len = size;
}

// Marks the chunk as allocated
void MarkAsAllocated(header* h) {
    h->isFree = 0;
}

// Sets the size of the next chunk in the header
void SetNextChunkSize(header* h, int size) {
    if (h->next != NULL) {
        h->next->len = size;
    }
}

// Checks if the next chunk is uninitialized
int NextChunkIsUninitialized(header* h) {
    return (h->next == NULL);
}
header* GetNextChunk(header* h) {
    return h->next;
}
void coalesceFreeChunks() {
    header* current = (header*)memoryStart;
    while (current != NULL && current < (header*)memoryEnd) {
        // Check if the current chunk and the next chunk are free
        if (current->isFree && current->next != NULL && current->next->isFree) {
            // Merge the current chunk with the next chunk
            current->len += current->next->len + sizeof(header);
            current->next = current->next->next;
        } else {
            // Move to the next chunk
            current = current->next;
        }
    }
}

void initializeMemory() {
    header* start = (header*)memory;
    start->len = MEMSIZE - sizeof(header);
    start->isFree = 1;
    start->next = NULL;
}

void *mymalloc(size_t size, char *file, int line){
    static int isMemoryInitialized = 0;
    if (!isMemoryInitialized) {
        initializeMemory();
        isMemoryInitialized = 1;
    }

    if (size == 0) {
        fprintf(stderr, "Error: cannot allocate 0 bytes\n");
        return NULL;
    }
    // printf("INITIAL SIZE: %zu", size);
    size = ROUNDUP8(size);
    // printf("SIZE AFTER: %zu", size);
    void* res = NULL;
    header* start = (header*)memoryStart;

    while ((void*)start < memoryEnd) {
        int chunkSize = GetChunkSize(start);
        bool isFree = IsFree(start);

        // this is where the comments below would've been before I changed them up
        if (isFree && chunkSize >= size + 8) {
            if (chunkSize >= size + sizeof(header) + 8) {
                header* newHeader = (header*)((char*)start + sizeof(header) + size);
                newHeader->len = chunkSize - size - sizeof(header);
                newHeader->isFree = 1;
                newHeader->next = start->next;
                start->next = newHeader;
            }
            start->len = size;
            MarkAsAllocated(start);
            return (char*)start + sizeof(header);
        }

        if (isFree == 0 || chunkSize < size + 8) {
            start = GetNextChunk(start);
        }
    }

    fprintf(stderr, "Error: not enough memory\n");
    return NULL;

}
//// if (chunkSize == 0 && isFree == false) {
        //     SetChunkSize(start, size + 8);
        //     MarkAsAllocated(start);
        //     res = start + 8;
        //     isFree = true;
        //     SetNextChunkSize(start, memoryEnd - (start + size + 8));
        //     return res;
        // }

        // if (isFree == false && chunkSize >= size + 8) {
        //     SetChunkSize(start, size + 8);
        //     MarkAsAllocated(start);
        //     res = start + 8;
        //     if (NextChunkIsUninitialized(start)) {
        //         SetNextChunkSize(start, chunkSize - (size + 8));
        //     }
        //     return res;
        // }