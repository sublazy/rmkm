#ifndef UTILS_H
#define UTILS_H

#include <linux/kernel.h>

#define ASSERT_RMKM(expr) \
    do { \
        if (expr) break; \
        printk(KERN_WARNING "RMKM: ASSERT fail: file %s, line %d, expr: " \
                             #expr, __FILE__, __LINE__); \
    } while (0);

#endif
