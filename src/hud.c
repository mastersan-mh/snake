/*
 * hud.c
 *
 *  Created on: 22 авг. 2018 г.
 *      Author: mastersan
 */

#include "hud.h"

#include "render.h"

#include <stdio.h>
#include <stdarg.h>

void hud_print(int x, int y, int atr, const char * format, ...)
{
#undef BUFSIZE
#define BUFSIZE (160)
    char text[BUFSIZE];

    va_list args;
    va_start(args, format);
    vsnprintf(text, BUFSIZE, format, args);
    va_end(args);

    text[BUFSIZE - 1] = '\0';

    render_add_text(x, y, atr, text);
}


void hud_print_centerscreen(size_t text_width, int atr, const char * format, ...)
{
#undef BUFSIZE
#define BUFSIZE (160)
    char text[BUFSIZE];

    va_list args;
    va_start(args, format);
    vsnprintf(text, BUFSIZE, format, args);
    va_end(args);

    text[BUFSIZE - 1] = '\0';

    render_add_text((VID_SCR_HEIGHT / 2), (VID_SCR_WIDTH - text_width) / 2, atr, text);
}

void hud_putch(int x, int y, int atr, char ch)
{
#undef BUFSIZE
#define BUFSIZE (2)
    char text[BUFSIZE];

    switch((unsigned char)ch)
    {
        case 0   : ch = ' '; break;
        case 176 : ch = '#'; break;
        default:
            break;
    }

    text[0] = ch;
    text[1] = '\0';

    hud_print(x, y, atr, text);
}
