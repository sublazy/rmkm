#include <linux/kernel.h>
#include <linux/ctype.h>
#include <linux/string.h>
#include "int_parser.h"

/* Private constants
 * -------------------------------------------------------------------------- */
#define MAX_INT_CHARS   32

/* Private types
 * -------------------------------------------------------------------------- */

/* Private data
 * -------------------------------------------------------------------------- */
static char input_buf[MAX_INT_CHARS] = {0};
static unsigned int idx = 0;
static int parsed_num = 0;
static bool new_num_ready = false;

/* Private functions
 * -------------------------------------------------------------------------- */
static int parse(char *buf)
{
    // Null termination required by kstrtoint.
    buf[MAX_INT_CHARS - 1] = 0;

    unsigned int base = 10;
    int result = 0;
    int err = kstrtoint(buf, base, &result);

    // Not really needed, as we only supply verified bytes to the parser.
    // Also, input from user space should always be valid. So to be extra-safe
    // let's just print warnings if something isn't right.
    if (err != 0) {
        printk(KERN_WARNING "RMKM: error on parsing int: %s\n", buf);
    }

    return result;
}

static void parser_reset(void)
{
    memset(input_buf, 0, MAX_INT_CHARS);
    idx = 0;
    new_num_ready = false;
}

/* Public functions
 * -------------------------------------------------------------------------- */
void int_parser_feed(char c)
{
    static char cprev = ' ';

    if (isdigit(c) || ((c == '-') && (cprev = ' '))) {
        input_buf[idx] = c;
        idx++;
    }

    if (isspace(c) && isdigit(cprev)) {
        parsed_num = parse(input_buf);
        new_num_ready = true;
    }

    cprev = c;
    // TODO Handle buffer overflows.
}

bool int_parser_is_num_ready(void)
{
    if (new_num_ready)
        return true;
    else
        return false;
}

// Note: Getting the number resets the parser.
int int_parser_get_num(void)
{
    parser_reset();
    return parsed_num;
}

