#ifndef HEAP_H
#define HEAP_H

// Total number of heaps available in the pool.
#define HEAPS_MAX 2

typedef struct heap_s heap_t;

// TODO Implement
enum heap_flavour {
    MAX_HEAP,   // The largest element is the root node.
    MIN_HEAP,   // The smallest element is the root node.
};

// Instantiate a new, empty heap. Allocate `initial_size` of memory for it.
heap_t *heap_new(enum heap_flavour flavour);

// Delete the heap and free the memory.
void heap_delete(heap_t *heap);

// Return the value of root node, without popping.
long heap_peek(heap_t *heap);

// Return the value of root node; delete the node.
long heap_pop(heap_t *heap);

// Add a new node.
void heap_push(heap_t *heap, long n);

// Tell if the heap is empty.
bool heap_is_empty(heap_t *heap);

// Return number of nodes stored by the heap.
unsigned int heap_cnt(heap_t *heap);

// Debug print.
void heap_dump(heap_t *heap);

#endif // HEAP_H
