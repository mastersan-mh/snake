/**
 * Strings and chars processing
 */
#ifndef STR_H_
#define STR_H_

#include <inttypes.h>
#include <unistd.h>

ssize_t str_char_find1st(const char *s, char chr);

char *str_WORD2strDEC(char *s, uint16_t value);

#endif
