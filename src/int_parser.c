#include <linux/ctype.h>
#include "int_parser.h"

/* Private constants
 * -------------------------------------------------------------------------- */
#define MAX_INT_CHARS   32

/* Private types
 * -------------------------------------------------------------------------- */

/* Private data
 * -------------------------------------------------------------------------- */
//static char input_buf[MAX_INT_CHARS];
//static unsigned int idx = 0;
static char prev_char = 0;

/* Private functions
 * -------------------------------------------------------------------------- */

/* Public functions
 * -------------------------------------------------------------------------- */
void int_parser_feed(char c)
{
    if (isspace(c))
        return;

    prev_char = c;
}

bool int_parser_is_num_ready(void)
{
    return false;
}

// Getting the number resets the parser.
int int_parser_get_num(void)
{
    return 0;
}

