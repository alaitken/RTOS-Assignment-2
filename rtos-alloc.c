#include "rtos-alloc.h"
#include <sys/mman.h>
#include <stdio.h>

typedef struct allocation {
	void	*a_base;
	size_t	a_len;

	struct allocation *a_prev;
	struct allocation *a_next;
} alloc_t;

alloc_t* head;
alloc_t* tail;

/*
 * The following functions are declartions of standard C library functions
 * for you to implement:
 */

/**
 * Allocate @b size bytes of memory for the exclusive use of the caller,
 * as `malloc(3)` would.
 */
void*	rtos_malloc(size_t size) {
    alloc_t* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    ptr->a_len = size;
    ptr->a_base = ptr;

    if (head == NULL) {
        head = ptr;
    } else {
        alloc_t* current = head;
        // Iterate to end of allocation list
        while (current->a_next != NULL) {
            current = current->a_next;
        }
        current->a_next = ptr;
        ptr->a_prev = current;
    }
    return ptr;
}

/**
 * Change the size of the allocation starting at @b ptr to be @b size bytes,
 * as `realloc(3)` would.
 */
void*	rtos_realloc(void *ptr, size_t size) {
    // void* newPtr = mremap(ptr, rtos_alloc_size(ptr), size);
    void* newPtr;
    return newPtr;
}

/**
 * Release the memory allocation starting at @b ptr for general use,
 * as `free(3)` would.
 */
void	rtos_free(void *ptr) {
    const int hasUnmapped = munmap(ptr, rtos_alloc_size(ptr));
    // if (hasUnmapped == -1) {
    //     printf("\n Failure freeing memory");
    // } else {
    //     printf("\n The memory has been freed");
    // }
}


/*
 * The following functions will help our test code inspect your allocator's
 * internal state:
 */

/**
 * How large is the allocation pointed at by this pointer?
 *
 * @pre   @b ptr points at a valid allocation (according to `rtos_allocated`)
 */
size_t	rtos_alloc_size(void *ptr) {
    if (!rtos_allocated(ptr)) return 0;

    alloc_t* alloc = (alloc_t*) ptr;
    return alloc->a_len;
}

/**
 * Does this pointer point at the beginning of a valid allocation?
 *
 * @param   ptr    any virtual address
 *
 * @returns whether or not @b ptr is a currently-allocated address returned
 *          from @b my_{m,re}alloc
 */
bool	rtos_allocated(void *ptr) {
    alloc_t* current = head;
    // Iterate to end of allocation list
    while (current->a_next != NULL) {
        if (current->a_base == ptr) return true;
        current = current->a_next;
    }
    return false;
}

/**
 * How much memory has been allocated by this allocator?
 *
 * @returns the number of bytes that have been allocated to user code,
 *          **not** including any allocator overhead
 */
size_t	rtos_total_allocated(void) {
    size_t total_allocated = 0;
    alloc_t* current = head;

    // Iterate to end of allocation list
    while (current->a_next != NULL) {
        total_allocated += head->a_len;
        current = current->a_next;
    }

    return total_allocated;
}