#include <stdio.h>
#include <stdlib.h>
#include "malloc.h"

#define MEMSIZE 4096  // or whatever size you need


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
void *mymalloc(size_t size, char *file, int line){
    if(size==0){
        print
    }

}