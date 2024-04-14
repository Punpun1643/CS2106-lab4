#include <stdio.h>
#include <assert.h>
#include "mymalloc.h"

void *runmalloc(size_t len, long expected_ndx)
{

    long ndx;
    void *ptr;

    printf("Allocate %ld bytes.\n", len);
    printf("Before: ");
    print_memlist();
    printf("\n");
    ptr = mymalloc(len);
    ndx = get_index((void *)ptr);
    printf("EXPECTED: %ld ACTUAL: %ld\n", expected_ndx, ndx);
    printf("After:  ");
    print_memlist();
    printf("\n");
    assert(ndx == expected_ndx);
    return ptr;
}

void runfree(void *ptr)
{
    printf("Free\n");
    printf("Before: ");
    print_memlist();
    printf("\n");
    myfree(ptr);
    printf("After:  ");
    print_memlist();
    printf("\n");
}

int main()
{
    void *ptr1, *ptr2, *ptr3, *ptr4, *ptr5;

    printf("Starting next-fit memory allocation test...\n");

    // Allocate some blocks.
    ptr1 = runmalloc(4, 0);  // Allocate block at the start.
    ptr2 = runmalloc(32, 4); // Allocate block immediately after ptr1.
    ptr3 = runmalloc(2, 36); // Allocate block immediately after ptr2.

    // Free the second block, ptr2
    runfree(ptr2); // This creates a free space after the first block.

    // Next allocation should start after the last allocated block (ptr3).
    ptr4 = runmalloc(12, 38); // This should go to the space after ptr3.

    // Free the third block, ptr3. Now there are two free blocks: one after ptr1 and one after ptr4.
    runfree(ptr3);

    ptr5 = runmalloc(2, 50); 

    // Free all blocks and print the final memory list state.
    runfree(ptr1);
    runfree(ptr4);
    runfree(ptr5);

    printf("Final memory list:\n");
    print_memlist();

    return 0;
}
