/**
 * Strings and chars processing
 */

#include "str.h"

#include <stdio.h>
#include <string.h>

/////////////////////////////////////////////////
//проверка первого вхождения символа
//вход:
//*s            -строка с символами
//chr           -проверяемый символ
//выход:
//=<x>          -символ chr входит в строку s в позиции <x>
//=-1           -символ chr НЕ входит в строку s
/////////////////////////////////////////////////
ssize_t str_char_find1st(const char *s, char chr)
{
    char * tmp;
    tmp = strchr(s, chr);
    return tmp == NULL? -1 : tmp - s;
}

//
//число в строку
//

////////////////////////////////////////////////
//преобразует WORD-число в DEC-строку
//вход:
//value         -число
//выход:
//*str          -DEC-строка
//=str
////////////////////////////////////////////////
char *str_WORD2strDEC(char *str, uint16_t value)
{
    sprintf(str, "%d", value);
    return str;
}
