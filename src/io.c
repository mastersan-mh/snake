/*
 * input.c
 *
 *  Created on: 29 дек. 2017 г.
 *      Author: mastersan
 */

#include "io.h"

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <stdio.h>


#include <locale.h>
#include <ncurses.h>

int io_init(void)
{
    /* language[_territory][.codeset][@modifier] */
    static const char locale[] = "ru_RU.utf8";
    //static const char locale[] = "";


    /*
    initscr is normally the first curses routine to call when initializing a program.
    A few special routines sometimes need to be called before it;
    these are slk_init, filter, ripoffline, use_env.
    For multiple-terminal applications, newterm may be called before initscr.

    http://invisible-island.net/ncurses/man/curs_initscr.3x.html
    */
    setlocale(LC_ALL, locale);
    WINDOW * w = initscr();

    raw();
    noecho();
    keypad(w, true);


    return 0;
}

void io_done(void)
{
    endwin();
}

int io_render_end(void)
{
    refresh();
    return 0;
}

int io_getch(void)
{
    return getch();
}

