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
static void insert(int n)
{
    heap_push(heap_l, n);
    heap_dump(heap_l);
}

/* Public functions
 * -------------------------------------------------------------------------- */
void median_calc_init(void)
{
    heap_l = heap_new();
    heap_r = heap_new();

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

    int median = heap_pop(heap_l);
    heap_dump(heap_l);

    return median;
}

void median_calc_dbg_print(void)
{
}
