#include <stdbool.h>

bool is_even(int n)
{
    if (n % 2 == 0)
        return true;
    else
        return false;
}

bool is_odd(int n)
{
    return !is_even(n);
}

