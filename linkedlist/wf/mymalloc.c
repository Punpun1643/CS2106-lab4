#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "llist.h"

char _heap[MEMSIZE] = {0};
TNode *_memlist = NULL; // To maintain information about length

void init_memory() {
  TData *initialData = (TData *) malloc(sizeof(TData));
  initialData->val = MEMSIZE;
  initialData->start_index = 0;
  initialData->is_allocated = false;
  TNode *initialNode = make_node(0, initialData);
  insert_node(&_memlist, initialNode, ASCENDING); 
}

// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
long get_index(void *ptr) {
    if(ptr == NULL)
        return -1;
    else
        return (long) ((char *) ptr - &_heap[0]);
}

void print_node(TNode *node) {
  if (node != NULL) {
    TData *data = node->pdata;
    printf("Status: %s Start index: %d Length: %d\n", data->is_allocated ? "ALLOCATED" : "FREE", data->start_index, data->val);
  }
}

void print_memlist() {
  process_list(_memlist, print_node);   
}
// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
  // traverse the linkedlist to and get the size
  // if size is greater than the input size, allocate
  if (_memlist == NULL) {
    init_memory(); 
  }

  TNode *worst_fit = NULL;
  size_t max_size = 0; 

  TNode *curr = _memlist;
  
  while (curr != NULL) {
    if (!curr->pdata->is_allocated && (size_t) curr->pdata->val >= size) { // found a large enough block
      if (curr->pdata->val == size) { // size equal, don't need to split
        curr->pdata->is_allocated = true;
        return (void*) &_heap[curr->pdata->start_index]; 
      } else if (curr->pdata->val > max_size) { // update worst_fit block
        max_size = curr->pdata->val;
        worst_fit = curr;
      }
    }
    curr = curr->next;
  }
  
  if (worst_fit != NULL) {
    TData *newData = (TData*) malloc(sizeof(TData));
    newData->val = worst_fit->pdata->val - size; // remaining free memory
    newData->is_allocated = false;
    newData->start_index = worst_fit->pdata->start_index + size;

    TNode *newNode = make_node(worst_fit->pdata->start_index + size, newData);
   
    // update info of the curr node
    worst_fit->pdata->val = size; // allocate size memory to the curr free node
    worst_fit->pdata->is_allocated = true;
    
    // insert new node to end of llist
    insert_node(&_memlist, newNode, ASCENDING);
    return (void*)&_heap[worst_fit->pdata->start_index]; 
  }

  return NULL;
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
  if (ptr == NULL) {
    return;
  }
  long index = get_index(ptr); 
  
  // get the node to be freed
  TNode *curr = find_node(_memlist, index);
  
  if (curr == NULL || !curr->pdata->is_allocated) {
    return; 
  }
  
  // free the node
  curr->pdata->is_allocated = false;
  
  // try to merge with succeeding block
  if (curr->next != NULL && !curr->next->pdata->is_allocated) {
    curr->pdata->val += curr->next->pdata->val; 
    merge_node(_memlist, curr, SUCCEEDING); 
  }

  // try merge with prev block
  if (curr->prev != NULL && !curr->prev->pdata->is_allocated) {
    curr->prev->pdata->val += curr->pdata->val; // update size of combined blocks
    merge_node(_memlist, curr, PRECEDING);
  }
}

