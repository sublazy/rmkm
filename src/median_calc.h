#ifndef MEDIAN_CALC_H
#define MEDIAN_CALC_H

void median_calc_feed(int number);

// This function will only return integers, but will tell you when the result
// is actually a float by setting the `is_result_float` flag to `true`.
// In such case, the actual result is equal to returned value + 0.5.
int median_calc_get_result(bool *is_result_float);

void median_calc_dbg_print(void);

#endif // MEDIAN_CALC_H
