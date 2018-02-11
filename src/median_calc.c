// This median calculator uses a super-simple algorithm of just storing
// counters tracking occurence of each number on the input.
//
// Pros:
//  - Fast (if input range is reasonably limited).
//  - Constant memory use, only static allocation.
//  - Can handle large amounts of input data.
//  - Constant performance, independent of size of the data set.
//
// Cons:
//  - Only suitable for very limited range of input data.
//  - Inefficient use of memory (sparsely populated array).

#include <linux/kernel.h>
#include <linux/bug.h>
#include <stdbool.h>

#include "utils.h"

/* Constants
 * -------------------------------------------------------------------------- */
#define INPUT_LIMIT_UPPER   (10)
#define INPUT_LIMIT_LOWER   0
#define INPUT_RANGE  (INPUT_LIMIT_UPPER - INPUT_LIMIT_LOWER)

/* Private Data
 * -------------------------------------------------------------------------- */
// Our primary data store. Counts how many times any input number occured.
int occurences[INPUT_RANGE];

// Number of numbers supplied so far.
unsigned int cnt_total = 0;

unsigned int cnt_left_half = 0;

/* Private functions
 * -------------------------------------------------------------------------- */
static bool is_number_in_range(int n)
{
    if ((n >= INPUT_LIMIT_LOWER) && (n <= INPUT_LIMIT_UPPER))
        return true;
    else
        return false;
}

static bool is_even(int n)
{
    if (n % 2 == 0)
        return true;
    else
        return false;
}

static bool is_odd(int n)
{
    return !is_even(n);
}

/* Public functions
 * -------------------------------------------------------------------------- */
void median_calc_feed(int n)
{
    if (!is_number_in_range(n)) {
        printk(KERN_WARNING "RMKM: Input number out of range: %d\n", n);
        return;
    }

    cnt_total++;
    occurences[n]++;
    cnt_left_half = cnt_total / 2;
}

int median_calc_get_result(bool *is_result_float)
{
    ASSERT_RMKM(is_result_float != NULL);
    *is_result_float = false;
    int median = 0;

    // Counter of numbers on the left from median.
    unsigned int cnt_left_scanned = 0;

    // For the case of even number of elements in the data set, let's find
    // two middle values.
    int median_l = 0, median_r = 0;

    for (unsigned int n = 0; n < INPUT_RANGE; n++) {
        cnt_left_scanned += occurences[n];
        if (cnt_left_scanned > cnt_left_half) {
            median = n;
            median_r = n;
            if (occurences[n] > 1) {
                median_l = n;
            }
            break;
        }

        if (occurences[n] > 0) {
            median_l = n;
        }
    }

    if (is_even(cnt_total)) {
        if (median_l != median_r) {
            median = (median_l + median_r) / 2;

            if (is_odd(median_l + median_r)) {
                *is_result_float = true;
            }
        }
    }

    return median;
}

void median_calc_dbg_print(void)
{
    printk(KERN_INFO "RMKM: Received numbers cnt: %d\n", cnt_total);
    printk(KERN_INFO "RMKM: Numbers to the left of the median: %d\n", cnt_left_half);
    for (unsigned int i = 0; i < INPUT_RANGE; i++) {
        printk(KERN_INFO "RMKM: cnt [%d]:\t%d\n", i, occurences[i]);
    }
}
