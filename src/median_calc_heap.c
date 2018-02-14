// This median calculator sorts received data into two heaps.

#include <linux/kernel.h>
#include <linux/bug.h>
#include <stdbool.h>

#include "utils.h"
#include "heap.h"

/* Constants
 * -------------------------------------------------------------------------- */

/* Private Data
 * -------------------------------------------------------------------------- */
// Number of numbers supplied so far.
unsigned int cnt_total = 0;

unsigned int cnt_left_half = 0;

// Our primary data stores: heap-left and -right
heap_t *heap_l = NULL;
heap_t *heap_r = NULL;

/* Private functions
 * -------------------------------------------------------------------------- */
// Insert a new input number into sorted data store.
static void insert(int val)
{
    // This implementation is not yet optimal. Instead of blindly pushing
    // always to heap_l first, let's push to the more appropriate (sort-wise)
    // heap (therefore taking care of the sort invariant), and then swap root
    // nodes only if the balance invariant is violated.
    //
    // It will still be O(n log n), but we'll reduce the constant factor.
    //
    // Anyway, this optimization won't matter until we can handle large amounts
    // of data.

    if (is_even(cnt_total)) {
        heap_push(heap_l, val);

        if (heap_cnt(heap_l) == 1) {
            // I could just return, but I want to assert the size invariant
            // after each insert.
            goto done;
        }

        if (heap_peek(heap_l) > heap_peek(heap_r)) {
            long val_to_r = heap_pop(heap_l);
            long val_to_l = heap_pop(heap_r);
            heap_push(heap_l, val_to_l);
            heap_push(heap_r, val_to_r);
        }

    // In case heaps are assymetrical
    } else {
        heap_push(heap_l, val);
        long val_to_r = heap_pop(heap_l);
        heap_push(heap_r, val_to_r);
    }

done:
    // Make sure that both heaps have (almost) the same number of nodes.
    ASSERT_RMKM((heap_cnt(heap_l) - heap_cnt(heap_r) == 0) ||
                (heap_cnt(heap_l) - heap_cnt(heap_r) == 1));

    //heap_dump(heap_l);
    //heap_dump(heap_r);
}

/* Public functions
 * -------------------------------------------------------------------------- */
size_t median_calc_get_name(char *name_buf)
{
    return sprintf(name_buf, "two heaps");
}

void median_calc_init(void)
{
    heap_l = heap_new(MAX_HEAP);
    heap_r = heap_new(MIN_HEAP);

    ASSERT_RMKM(heap_l != NULL);
    ASSERT_RMKM(heap_r != NULL);
}

void median_calc_cleanup(void)
{
    heap_delete(heap_l);
    heap_delete(heap_r);
}

void median_calc_feed(int n)
{
    insert(n);
    cnt_total++;
    cnt_left_half = cnt_total / 2;
}

// TODO It became long and complex. Refactor.
size_t median_calc_get_result(char *ans_buf)
{
    ASSERT_RMKM(ans_buf != NULL);

    bool is_float = false;
    bool is_nan = false;
    long median = 0;

    if (is_odd(cnt_total)) {
        median = heap_peek(heap_l);
    } else {
        long median_l = heap_peek(heap_l);
        long median_r = heap_peek(heap_r);

        if (median_l + median_r == -1) {
            // Special snowflake case. -1/2 wouldn't give "-0" answer.
            size_t len = sprintf(ans_buf, "-0.5\n");
            return len;
        }

        if (is_odd(median_l + median_r)) {
            is_float = true;
        }

        median = (median_l + median_r) / 2;
    }

    if (cnt_total == 0) {
        is_nan = true;
    }

    size_t len = 0;
    if (is_nan) {
        len = sprintf(ans_buf, "NaN\n");
    } else {
        if (is_float) {
            len = sprintf(ans_buf, "%ld.5\n", median);
        } else {
            len = sprintf(ans_buf, "%ld.0\n", median);
        }
    }

    return len;
}

void median_calc_dbg_print(void)
{
}
