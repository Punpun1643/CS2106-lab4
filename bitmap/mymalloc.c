#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "bitmap.h"

char _heap[MEMSIZE] = {0};
int sizeMemoryAllocated[MEMSIZE] = {0};

// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
long get_index(void *ptr) {
    if(ptr == NULL)
        return -1;
    else
        return (long) ((char *) ptr - &_heap[0]);
}

void print_memlist() {
  // Implement this to call print_map from bitmap.c
  print_map(_heap, MEMSIZE/8);
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
  int index = search_map(_heap, MEMSIZE/8, size);

  if (index == -1) { // not found
    return NULL;
  } else {
     allocate_map(_heap, index, size);
     sizeMemoryAllocated[index] = size; // record the size of contiguous memory allocated
     return _heap + index;
  }
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
  if (ptr == NULL) {
    return; 
  }

  // check if ptr exceeds the heap size
  if (ptr < (void*)_heap || ptr > (void*)(_heap + MEMSIZE)) {
    return; 
  }

  long index = get_index(ptr);
  free_map(_heap, index, sizeMemoryAllocated[index]);
}

