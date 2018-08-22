/**
 * text
 */
#include "_text.h"

struct TTEXT text = {};

/**
 * инициализация текстового режима
 */
void text_init80X25X8(void)
{
    text.c.chr=0x00;
    text.c.atr=0x00;
}

/**
 * заполнить экран текущими символами и атрибутами
 */
void text_fill_screen(void)
{
/*
TODO:
    Please try bkgd, or wbkgd for specifying a window.
    First you have to enable color support with start_color().
    And then define color pair. Example:init_pair(1,COLOR_BLUE, COLOR_RED)
    The order is pair_number, foreground, background
    Finally, set colors: wbkgd(WindowName, COLOR_PAIR(1)).
*/
/*
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_RED);
    bkgd(COLOR_PAIR(1)).
    */

    erase();
}

