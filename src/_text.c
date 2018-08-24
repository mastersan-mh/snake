/**
 * text
 */
#include "_text.h"

#include <curses.h>
/**
 * заполнить экран текущими символами и атрибутами
 */
void text_fill_screen(int atr, int ch)
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

