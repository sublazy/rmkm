// This median calculator sorts received data into two heaps.

#include <linux/kernel.h>
#include <linux/bug.h>
#include <stdbool.h>

#include "utils.h"
#include "heap.h"

/* Constants
 * -------------------------------------------------------------------------- */
#define INPUT_LIMIT_UPPER   (255)
#define INPUT_LIMIT_LOWER   (-256)
#define INPUT_SPREAD  (INPUT_LIMIT_UPPER - INPUT_LIMIT_LOWER + 1)

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
// TODO Move to utils.
static bool is_number_in_range(int n)
{
    if ((n >= INPUT_LIMIT_LOWER) && (n <= INPUT_LIMIT_UPPER))
        return true;
    else
        return false;
}

// Insert a new input number into sorted data store.
static void insert(int val)
{
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
    // TODO Remove this limit. Pointless in the heap algo.
    if (!is_number_in_range(n)) {
        printk(KERN_WARNING "RMKM: Input number out of range: %d\n", n);
        return;
    }

    insert(n);
    cnt_total++;
    cnt_left_half = cnt_total / 2;
}

int median_calc_get_result(bool *is_result_float, bool *is_result_nan)
{
    ASSERT_RMKM(is_result_float != NULL);
    ASSERT_RMKM(is_result_nan != NULL);

    *is_result_float = false;
    *is_result_nan = false;

    if (cnt_total == 0) {
        *is_result_nan = true;
        return 0;
    }

    long median = 0;
    if (is_odd(cnt_total)) {
        median = heap_peek(heap_l);
    } else {
        long median_l = heap_peek(heap_l);
        long median_r = heap_peek(heap_r);

        if (is_odd(median_l + median_r)) {
            *is_result_float = true;
        }

        median = (median_l + median_r) / 2;
    }

    return median;
}

void median_calc_dbg_print(void)
{
}
