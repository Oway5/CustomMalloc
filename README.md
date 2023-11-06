**Rayyan & Oways Malloc Project**

Your test plan:
- The test plan entails stress testing the two functions with several types of arguments. The types of these arguments are 1 byte objects, 7 byte objects, testing to see if the assigned value is accessible and accurate, and checking to see if one of the error cases actually functions properly.

Descriptions of your test programs (including arguments), 
- Task 1 - malloc() and immediately free() a 1-byte object, 120 times
- Task 2 - Use malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to deallocate the chunks
- Task 3 - Create an array of 120 pointers. Repeatedly make a random choice between allocating a 1-byte object and adding the pointer to the array and deallocating a previously allocated object (if any), until you have allocated 120 times. Deallocate any remaining objects.
- Task 4 - Allocate 7 byte objects repeatedly 120 times and free them.
- Task 5 - Allocate a value to my pointer from a variable and print it.
- Task 6 - Free an already free pointer.
