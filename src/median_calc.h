#ifndef MEDIAN_CALC_H
#define MEDIAN_CALC_H

void median_calc_init(void);

void median_calc_cleanup(void);

size_t median_calc_get_name(char *name_buf);

void median_calc_feed(int number);

// Put the median (as a properly formatted string) in the buffer.
// Return length of the produced string.
size_t median_calc_get_result(char *ans_buf);

void median_calc_dbg_print(void);

#endif // MEDIAN_CALC_H
