#ifndef INT_PARSER_H
#define INT_PARSER_H

#include <stdbool.h>

void int_parser_feed(char c);
bool int_parser_is_num_ready(void);
int int_parser_get_num(void);

#endif // INT_PARSER_H
