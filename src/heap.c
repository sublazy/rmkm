// Naming conventions:
// idx  -- index
//
// By default, when the code says "node" it means a reference (location) of a
// node, not its value. When the code means "value of a node", it will be
// stated explicitly, e.g: value(node), or value(parent).
//
// Such convention was chosen because this implementation is concerned with
// indexes a lot more often than it is with values. So the code should a bit
// more readable.

#include <linux/kernel.h>
#include <linux/gfp.h>
#include <linux/slab.h>

#include "heap.h"
#include "utils.h"

#define ROOT    0

/* Private types
 * -------------------------------------------------------------------------- */
// A node is just an index pointing to a location in the data array.
typedef unsigned int node_t;

struct heap_s {
    long *data;         // The data array.
    size_t len;         // Length of the data array.
    unsigned int cnt;   // Number of elements currently in the heap.

    // min-heap or max-heap
    enum heap_flavour flavour;

    // A method to find min- or max-child, depending on the flavour.
    node_t (*get_extreme_child)(heap_t *heap, node_t node);
};

/* Private prototypes
 * -------------------------------------------------------------------------- */
static node_t child_l(node_t node);
static node_t child_r(node_t node);
static node_t parent(node_t node);
static node_t children_cnt(heap_t *heap, node_t node);
static node_t get_min_child(heap_t *heap, node_t node);
static node_t get_max_child(heap_t *heap, node_t node);
static node_t last_node(heap_t *heap);
static long value(heap_t *heap, node_t node);
static void set_value(heap_t *heap, node_t node, long val);
static bool is_root(node_t node);
static bool is_order_ok(heap_t *heap, node_t parent, node_t child);
static void sift_up(heap_t *heap, node_t node);
static void sift_down(heap_t *heap, node_t node);
static void swap_values(heap_t *heap, node_t node_a, node_t node_b);

/* Private data
 * -------------------------------------------------------------------------- */
static struct heap_s heap_pool[HEAPS_MAX];
static node_t cnt_heaps_in_use = 0;

/* Private functions
 * -------------------------------------------------------------------------- */
static node_t child_l(node_t node)
{
    return 2 * node + 1;
}

static node_t child_r(node_t node)
{
    return 2 * node + 2;
}

static node_t parent(node_t node)
{
    return (node - 1) / 2;
}

// Returns number of children (0, 1 or 2) of a given node.
static node_t children_cnt(heap_t *heap, node_t node)
{
    unsigned int cnt = 0;

    if (child_l(node) <= last_node(heap))
        cnt++;

    if (child_r(node) <= last_node(heap))
        cnt++;

    return cnt;
}

// Return the child with the smallest value.
// Assumes the node has at least one child.
static node_t get_min_child(heap_t *heap, node_t node)
{
    ASSERT_RMKM(children_cnt(heap, node) > 0);

    if (children_cnt(heap, node) == 1) {
        return child_l(node);

    } else {
        long val_l = value(heap, child_l(node));
        long val_r = value(heap, child_r(node));

        if (val_l < val_r)
            return child_l(node);
        else
            return child_r(node);
    }
}

// Return the child with the biggest value.
// Assumes the node has at least one child.
static node_t get_max_child(heap_t *heap, node_t node)
{
    ASSERT_RMKM(children_cnt(heap, node) > 0);

    if (children_cnt(heap, node) == 1) {
        return child_l(node);

    } else {
        long val_l = value(heap, child_l(node));
        long val_r = value(heap, child_r(node));

        if (val_l > val_r)
            return child_l(node);
        else
            return child_r(node);
    }
}

static node_t last_node(heap_t *heap)
{
    ASSERT_RMKM(!heap_is_empty(heap));
    return heap->cnt - 1;
}

// Return value of a node
static long value(heap_t *heap, node_t node)
{
    ASSERT_RMKM(!heap_is_empty(heap));
    return heap->data[node];
}

// Set value of a node
static void set_value(heap_t *heap, node_t node, long val)
{
    heap->data[node] = val;
}

static bool is_root(node_t node)
{
    if (node == 0)
        return true;
    else
        return false;
}

static bool
is_order_ok(heap_t *heap, node_t node, node_t child)
{
    ASSERT_RMKM(node == parent(child));

    bool is_ok_for_max_heap = false;

    if (value(heap, node) >= value(heap, child))
        is_ok_for_max_heap = true;
    else
        is_ok_for_max_heap = false;

    if (heap->flavour == MAX_HEAP)
        return is_ok_for_max_heap;
    else
        return !is_ok_for_max_heap;
}

static void sift_up(heap_t *heap, node_t node)
{
    if (is_root(node))
        return;

    while (!is_order_ok(heap, parent(node), node)) {
        swap_values(heap, node, parent(node));
        node = parent(node);
        if (is_root(node))
            break;
    }
}

static void sift_down(heap_t *heap, node_t node)
{
    ASSERT_RMKM(!heap_is_empty(heap));

    if (children_cnt(heap, node) == 0)
        return;

    // extreme = smallest or largest, depending on the heap flavour.
    node_t extreme_child = heap->get_extreme_child(heap, node);

    while (!is_order_ok(heap, node, extreme_child)) {
        swap_values(heap, node, extreme_child);
        node = extreme_child;
        if (children_cnt(heap, node) == 0) {
            break;
        } else {
            extreme_child = heap->get_extreme_child(heap, node);
        }
    }
}

static void
swap_values(heap_t *heap, node_t node_a, node_t node_b)
{
    long a_tmp = heap->data[node_a];
    heap->data[node_a] = heap->data[node_b];
    heap->data[node_b] = a_tmp;
}

/* Public functions
 * -------------------------------------------------------------------------- */
heap_t *heap_new(enum heap_flavour flavour)
{
    // TODO Support multiple new/delete cycles.
    // Update: Why do I even use a static pool? I can just malloc new heaps.
    unsigned int new_heap_idx = cnt_heaps_in_use;
    cnt_heaps_in_use++;

    heap_t *new_heap = &heap_pool[new_heap_idx];

    // TODO Handle unsuccessful malloc.
    new_heap->len = 1024 * sizeof(long);
    new_heap->data = kmalloc_array(1024, sizeof(long), GFP_KERNEL);
    new_heap->cnt = 0;
    new_heap->flavour = flavour;

    switch (flavour) {
    case (MAX_HEAP):
        new_heap->get_extreme_child = get_max_child;
        break;
    case (MIN_HEAP):
        new_heap->get_extreme_child = get_min_child;
        break;
    default:
        printk(KERN_WARNING "RMKM: Wrong heap flavour!\n");
    }

    return new_heap;
}

void heap_delete(heap_t *heap)
{
    kfree(heap->data);
}

long heap_peek(heap_t *heap)
{
    return heap->data[ROOT];
}

long heap_pop(heap_t *heap)
{
    ASSERT_RMKM(!heap_is_empty(heap));

    long result = value(heap, ROOT);
    long last_leaf_val = value(heap, last_node(heap));

    set_value(heap, ROOT, last_leaf_val);
    sift_down(heap, ROOT);
    heap->cnt--;

    return result;
}

void heap_push(heap_t *heap, long n)
{
    unsigned int new_node_idx = heap->cnt;
    heap->data[new_node_idx] = n;
    sift_up(heap, new_node_idx);

    // TODO Signal a failure when the heap grows too big.
    heap->cnt++;
}

bool heap_is_empty(heap_t *heap)
{
    if (heap->cnt == 0)
        return true;
    else
        return false;
}

unsigned int heap_cnt(heap_t *heap)
{
    return heap->cnt;
}

void heap_dump(heap_t *heap)
{
    printk(KERN_INFO "RMKM: heap %d ----- \n", heap->flavour);
    for (unsigned int i = 0; i < heap->cnt; i++) {
        printk(KERN_INFO "RMKM: heap[%d]: %ld\n", i, heap->data[i]);
    }
}

