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
 * @brief Put the char with it attributes (clipped)
 * @param[in] x		coord. x
 * @param[in] y         coord. x
 */
void text_setch(int x, int y)
{
    int res = -1;
    char tmp[2] = {0, 0};
    switch(text.c.chr)
    {
        case 0   : res = mvprintw(y, x, " "); break;
        case 176 : res = mvprintw(y, x, "#"); break; /*  */
        default:
            tmp[0] = text.c.chr;
            res = mvprintw(y, x, tmp);
            break;
    }
    if(res != 0)
        fprintf(stderr, "res = %d\n", res);
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

