#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/gfp.h>

#include "heap.h"

/* Private constants
 * -------------------------------------------------------------------------- */
/* Private types
 * -------------------------------------------------------------------------- */
struct heap_s {
    long* data;             // The data array.
    size_t len;             // Length of the data array.
    unsigned int cnt;       // Number of elements currently in the heap.
};

/* Private data
 * -------------------------------------------------------------------------- */
static struct heap_s heap_pool[HEAPS_MAX];
static unsigned int cnt_heaps_in_use = 0;

/* Private functions
 * -------------------------------------------------------------------------- */

/* Public functions
 * -------------------------------------------------------------------------- */
heap_t *heap_new(void)
{
    // TODO Support multiple new/delete cycles.
    unsigned int new_heap_idx = cnt_heaps_in_use;
    cnt_heaps_in_use++;

    heap_t *new_heap = &heap_pool[new_heap_idx];

    new_heap->len = PAGE_SIZE;
    new_heap->data = kzalloc(new_heap->len, GFP_KERNEL);
    new_heap->cnt = 0;

    return new_heap;
}

void heap_delete(heap_t *heap)
{
    kfree(heap->data);
}

long heap_peek(heap_t *heap)
{
    return 0;
}

long heap_pop(heap_t *heap)
{
    return 0;
}

void heap_push(heap_t *heap, long n)
{
}

void heap_dump(heap_t *heap)
{
}

