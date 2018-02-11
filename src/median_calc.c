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
#include <stdbool.h>

/* Constants
 * -------------------------------------------------------------------------- */
#define INPUT_LIMIT_UPPER   (32)
#define INPUT_LIMIT_LOWER   0
#define INPUT_RANGE  (INPUT_LIMIT_UPPER - INPUT_LIMIT_LOWER)

/* Private Data
 * -------------------------------------------------------------------------- */
// Our primary data store. Counts how many times any input number occured.
int data[INPUT_RANGE];

// Number of numbers supplied so far.
unsigned int cnt = 0;

// Our final result -- median of numbers supplied so far.
// TODO Support float results.
int median = 0;

unsigned int cnt_lower = 0;

/* Private functions
 * -------------------------------------------------------------------------- */
bool is_number_in_range(int n)
{
    if ((n >= INPUT_LIMIT_LOWER) && (n <= INPUT_LIMIT_UPPER))
        return true;
    else
        return false;
}

/* Public functions
 * -------------------------------------------------------------------------- */
void median_calc_feed(int n)
{
    if (!is_number_in_range(n)) {
        printk(KERN_WARNING "RMKM: Input number out of range: %d\n", n);
        return;
    }

    cnt++;
    data[n]++;
    cnt_lower = cnt / 2;
}

int median_calc_get_result(void)
{
    unsigned int left_half_cnt = 0;

    for (unsigned int i = 0; i < INPUT_RANGE; i++) {
        left_half_cnt += data[i];
        // FIXME Cryptic
        if (left_half_cnt > cnt_lower) {
            median = i;
            break;
        }
    }

    return median;
}

void median_calc_dbg_print(void)
{
    printk(KERN_INFO "RMKM: Received numbers cnt: %d\n", cnt);
    printk(KERN_INFO "RMKM: Numbers to the left of the median: %d\n", cnt_lower);
    for (unsigned int i = 0; i < INPUT_RANGE; i++) {
        printk(KERN_INFO "RMKM: cnt [%d]:\t%d\n", i, data[i]);
    }
}
