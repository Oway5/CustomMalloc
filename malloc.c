#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <stdbool.h>

#define MEMSIZE 4096  //not sure if this is the size we want. we can also make a macro from the .h and make it a static var here
#define ROUNDUP8(x) (((x) + 7) & (-7))

static char memory[MEMSIZE];
void* memoryStart = (void*)memory;  // Start of  memory block
void* memoryEnd = (void*)(memory + MEMSIZE); // end of memory block



void myfree(void *ptr, char *file, int line){

    // Moves the pointer address back 8 bytes to start of the chunk's header.
    ptr = ptr - 8;
    copyPtr = headPtr;// Copies over start of linked list for iteration.

    case1 = 0;// Checks if Calling free() with an address not obtained from malloc()
    case2 = 0;// Checks if Calling free() with an address not at the start of a chunk.
    case3 = 1;// Checks if Calling free() a second time on the same pointer.

    // Checks if address is within array range.
    if(ptr >= headPtr || ptr <= headPtr + MEMLENGTH) {
        case1 = 1;
    }

    // Checks if address is the start of a chunk.
    while(copyPtr != NULL) {

        if(copyPtr == ptr) {
            case2 = 1;
            break;
        }
        copyPtr = copyPtr -> next;// Naviagates to next header.
    }

    // Checks if the header block is already free.
    if(ptr -> hasData == 0) {
        case3 = 0;
    }

    // If all error cases are mititgated.
    if(case1 && case2 && case3){
        // Changes the header's hasData field to false.
        ptr -> hasData = 0;
        // Calls method to combine free chunks.
        coalesceFreeChunks();

    }

    // Prints error statements.
    else if(case1 == 0) {
        printf("You called free() with an address that was not within the allocated memory block.");
    }
    else if(case2 == 0) {
        printf("You called free() with an address that was not at the start of the chunk.");
    }
    else if(case3 == 0) {
        printf("You called free() on a chunk that was already free.");
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

    size = ROUNDUP8(size);

    void* res = NULL;
    header* start = (header*)memoryStart;

    while (start < memoryEnd) {
        int chunkSize = GetChunkSize(start);
        bool isFree = IsFree(start);

        // if (chunkSize == 0 && isFree == false) {
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

        if (isFree == true || chunkSize < size + 8) {
            start = GetNextChunk(start);
        }
    }

    fprintf(stderr, "Error: not enough memory\n");
    return NULL;

}