#include <linux/kernel.h>

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
heap_t *heap_new(size_t initial_size)
{
    return NULL;
}

void heap_delete(heap_t *heap)
{
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

