//25.03.2004
//
//змея
//

#include "snaketypes.h"
#include "menu.h"
#include "game.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <ncurses.h>

#include "fakefunc.h"
#include "_time.h"
#include "_text.h"
#include "str.h"

int main()
{
    int quit=0;
    char line_st;
    char line_end;
    char cur_x,cur_y;
    text_init80X25X8();
    text.c.atr=0x00;
    text.c.chr=0x00;
    text_fill_screen();
    text_videopage_set(0);
    text_cursor_get(0,&line_st,&line_end,&cur_x,&cur_y);
    text_cursor_setxy(0,80,25);


    srand(time(NULL));

    chart_load();
    while(!quit)
    {
        quit = menu();
    }
    chart_save();
    text.c.atr=0x0F;
    text.c.chr=0x00;
    text_fill_screen();
    text_cursor_set(line_st,line_end);
    text_cursor_setxy(0,cur_x,cur_y);
    return(0);
}
