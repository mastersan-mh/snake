/**
 * Strings and chars processing
 */

#include "snaketypes.h"

#include <stdio.h>
#include <string.h>

/**
 * @brief Check the key is valid letter
 */
bool str_key_is_character(int key)
{
#if 0
    static const char valid_chars[] =
            "!#$%&'()+,-.0123456789;=@"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`"
            "abcdefghijklmnopqrstuvwxyz{}~"
            "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
            "абвгдежзийклмнопрстуфхцчшщъыьэюяЁё";
    return (strchr(valid_chars, key) != NULL);
#else
    return true;
#endif
}

/**
 * @brief Number to a decimal string
 */
char *str_WORD2strDEC(char *str, uint16_t value)
{
    sprintf(str, "%d", value);
    return str;
}
