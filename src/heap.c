#include <linux/kernel.h>
#include <linux/gfp.h>
#include <linux/slab.h>

#include "heap.h"
#include "utils.h"

/* Private prototypes
 * -------------------------------------------------------------------------- */
/* static unsigned int child_l(unsigned int idx_node); */
/* static unsigned int child_r(unsigned int idx_node); */
static unsigned int parent(unsigned int idx_node);
//static bool is_empty(heap_t *h);

static void
swap_values(heap_t *h, unsigned int idx_node_a, unsigned int idx_node_b);

static bool
is_order_ok(heap_t *h, unsigned int idx_parent, unsigned int idx_child);

/* Private types
 * -------------------------------------------------------------------------- */
struct heap_s {
    long *data;             // The data array.
    size_t len;             // Length of the data array.
    unsigned int cnt;       // Number of elements currently in the heap.
};

/* Private data
 * -------------------------------------------------------------------------- */
static struct heap_s heap_pool[HEAPS_MAX];
static unsigned int cnt_heaps_in_use = 0;

/* Private functions
 * -------------------------------------------------------------------------- */
/* static unsigned int child_l(unsigned int idx_node) */
/* { */
/*     return 2 * idx_node + 1; */
/* } */

/* static unsigned int child_r(unsigned int idx_node) */
/* { */
/*     return 2 * idx_node + 2; */
/* } */

static unsigned int parent(unsigned int idx_node)
{
    return (idx_node - 1) / 2;
}

static bool is_root(unsigned int idx_node)
{
    if (idx_node == 0)
        return true;
    else
        return false;
}

/* static bool is_empty(heap_t *h) */
/* { */
/*     if (h->cnt == 0) */
/*         return true; */
/*     else */
/*         return false; */
/* } */

static bool
is_order_ok(heap_t *h, unsigned int idx_parent, unsigned int idx_child)
{
    ASSERT_RMKM(parent(idx_child) == idx_parent);

    long parent = h->data[idx_parent];
    long child = h->data[idx_child];

    // TODO Handle -max and -min flavours.
    if (parent >= child)
        return true;
    else
        return false;
}

static void sift_up(heap_t *h, unsigned int idx_node)
{
    if (is_root(idx_node))
        return;

    unsigned int idx_parent = parent(idx_node);

    while (!is_order_ok(h, idx_parent, idx_node)) {
        swap_values(h, idx_node, idx_parent);
        idx_node = idx_parent;
        if (is_root(idx_node))
            break;

        idx_parent = parent(idx_node);
    }
}

static void
swap_values(heap_t *h, unsigned int idx_node_a, unsigned int idx_node_b)
{
    long a_tmp = h->data[idx_node_a];
    h->data[idx_node_a] = h->data[idx_node_b];
    h->data[idx_node_b] = a_tmp;
}

/* Public functions
 * -------------------------------------------------------------------------- */
heap_t *heap_new(void)
{
    // TODO Support multiple new/delete cycles.
    // Update: Why do I even use a static pool? I can just malloc new heaps.
    unsigned int new_heap_idx = cnt_heaps_in_use;
    cnt_heaps_in_use++;

    heap_t *new_heap = &heap_pool[new_heap_idx];

    new_heap->len = PAGE_SIZE;
    new_heap->data = kzalloc(new_heap->len, GFP_KERNEL);
    new_heap->cnt = 0;

    return new_heap;
}

void heap_delete(heap_t *h)
{
    kfree(h->data);
}

long heap_peek(heap_t *h)
{
    return h->data[0];
}

long heap_pop(heap_t *h)
{
    return h->data[0];
}

void heap_push(heap_t *h, long n)
{
    unsigned int new_node_idx = h->cnt;
    h->data[new_node_idx] = n;
    sift_up(h, new_node_idx);

    // TODO Resize when it grows too big.
    h->cnt++;
}

void heap_dump(heap_t *h)
{
    for (unsigned int i = 0; i < h->cnt; i++) {
        printk(KERN_INFO "RMKM: heap[%d]: %ld\n", i, h->data[i]);
    }
}

