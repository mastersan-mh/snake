/*
 * render.c
 *
 *  Created on: 21 авг. 2018 г.
 *      Author: mastersan
 */

#include "Z_mem.h"

#include "render.h"

#include "io.h"
#include "eng_curses.h"

#include <stdlib.h>

#include <signal.h>
#include <sys/ioctl.h>


#define RENDER_FAST
/* #define RENDER_USE_STDSCR */

#define RENDER_BGCOLORS_NUM 8
#define RENDER_FGCOLORS_NUM 8

#define RENDER_MODEL_LIST_SIZE (80 * 25 + 10)
#define RENDER_TEXT_LIST_SIZE 128

#include <stdio.h>

#define ERROR(format, ...) \
        fprintf(stderr, format, ##__VA_ARGS__)

#define R_PRINT(x, y, format, ...) \
        mvwprintw(ren.mainwindow, y, x, format, ##__VA_ARGS__)


struct render_model
{
    int x;
    int y;
    int bg_atr;
    const struct model * model;
};

struct render_text
{
    int x;
    int y;
    int bg_atr;
    char * text;
};

struct render
{
    WINDOW * mainwindow;

    bool bg_fill;
    int bg_atr;
    uint64_t bg_ch;

    size_t model_list_num;
    struct render_model model_list[RENDER_MODEL_LIST_SIZE];

    size_t text_list_num;
    struct render_text text_list[RENDER_TEXT_LIST_SIZE];

};

static struct render ren = {};

int render_init(void)
{
    /* TODO: ifdef color */
    start_color();

    int nlines = VID_SCR_HEIGHT;
    int ncols = VID_SCR_WIDTH;
    int begin_y = 0;
    int begin_x = 0;


#ifdef RENDER_USE_STDSCR
    ren.mainwindow = stdscr;
#else
    ren.mainwindow = newwin(nlines, ncols, begin_y, begin_x);
#endif

    if(ren.mainwindow == NULL)
    {
        ERROR("render_init() failed");
        return -1;
    }

    //getmaxyx(ren.mainwindow, max_y, max_x);

    /* like VGA colors */
    static const int bgcolors[RENDER_BGCOLORS_NUM] =
    {
            COLOR_BLACK,
            COLOR_BLUE,
            COLOR_GREEN,
            COLOR_CYAN,
            COLOR_RED,
            COLOR_MAGENTA,
            COLOR_YELLOW,
            COLOR_WHITE,
    };

    static const int fgcolors[RENDER_FGCOLORS_NUM] =
    {
            COLOR_BLACK,
            COLOR_BLUE,
            COLOR_GREEN,
            COLOR_CYAN,
            COLOR_RED,
            COLOR_MAGENTA,
            COLOR_YELLOW,
            COLOR_WHITE,
    };

    /* int cpairs = COLOR_PAIRS; */

    /* i = 1 .. COLOR_PAIRS-1 */
    int i = 1;
    int ibgcolor;
    int ifgcolor;
    for(ibgcolor = 0; ibgcolor < RENDER_BGCOLORS_NUM; ++ibgcolor)
    {
        for(ifgcolor = 0; ifgcolor < RENDER_FGCOLORS_NUM; ++ifgcolor)
        {
            init_pair(i, fgcolors[ifgcolor], bgcolors[ibgcolor]);
            ++i;
        }
    }

    return 0;
}

void render_done(void)
{
#ifdef RENDER_USE_STDSCR
#else
    delwin(ren.mainwindow);
#endif
}

void render_begin(void)
{
#ifdef RENDER_FAST
        werase(ren.mainwindow);
#else
        wclear(ren.mainwindow);
#endif
}

void render_end(void)
{
    wrefresh(ren.mainwindow);
}

void render_winch(void)
{
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    int ws_col = w.ws_col;
    int ws_row = w.ws_row;

    int cols_max;
    int rows_max;
    int cols;
    int rows;

#ifdef RENDER_USE_STDSCR
    cols_max = ws_row;
    cols = ws_col;
    rows_max = ws_col;
    rows = ws_row;
#else

    if(ws_col >= VID_SCR_WIDTH)
    {
        cols_max = ws_col;
        cols = VID_SCR_WIDTH;
    }
    else
    {
        cols_max = ws_col;
        cols = ws_col;
    }

    if(ws_row >= VID_SCR_HEIGHT)
    {
        rows_max = ws_row;
        rows = VID_SCR_HEIGHT;
    }
    else
    {
        rows_max = ws_row;
        rows = ws_row;
    }

#endif

    COLS = cols_max;
    LINES = rows_max;
    wresize(ren.mainwindow, rows, cols);

    wclear(ren.mainwindow);

}

/*
 * @note
 * atr:
 *   0xBA
 *   B - blink + bgcolor
 *   A - fgcolors
 */

int P_calculate_atr(uint8_t atr)
{
    int additional_atr = 0;

    uint8_t blink = ((atr & 0x80) >> 4);
    if(blink)
    {
        additional_atr = additional_atr | A_BLINK;
    }
    else
    {
        additional_atr = additional_atr | A_NORMAL;
    }
    uint8_t bg = ((atr & 0x70) >> 4);
    uint8_t fg = (atr & 0x0F);

    if(fg >= RENDER_FGCOLORS_NUM)
    {
        fg = fg & 0x07;
        additional_atr = additional_atr | A_BOLD;
    }

    int cpair = bg * RENDER_FGCOLORS_NUM + fg + 1;

    return COLOR_PAIR(cpair) | additional_atr;
};


static void P_atr_set(uint8_t atr)
{
    wattroff(ren.mainwindow, ~(attr_t)0);
    wattron(ren.mainwindow, P_calculate_atr(atr));
};

void render_clearbuf(void)
{
    size_t i;

    ren.bg_fill = false;

    /* clear models */
    ren.model_list_num = 0;

    /* clear text */
    for(i = 0; i < ren.text_list_num; ++i)
    {
        struct render_text * rt = &ren.text_list[i];
        Z_free(rt->text);
    }

    ren.text_list_num = 0;
}

void render(void)
{
    size_t i;

    if(ren.bg_fill)
    {
        wbkgdset(ren.mainwindow, ren.bg_ch | P_calculate_atr(ren.bg_atr));
#ifdef RENDER_FAST
        werase(ren.mainwindow);
#else
        wclear(ren.mainwindow);
#endif
    }

    /* render models */
    for(i = 0; i < ren.model_list_num; ++i)
    {
        struct render_model * rm = &ren.model_list[i];
        /* simple, print the string :) */
        P_atr_set(rm->bg_atr);
        R_PRINT(rm->x, rm->y + 1, "%s", rm->model->s);
    }

    /* render text */
    for(i = 0; i < ren.text_list_num; ++i)
    {
        struct render_text * rt = &ren.text_list[i];
        P_atr_set(rt->bg_atr);
        R_PRINT(rt->x, rt->y, "%s", rt->text);
    }
}

void render_background(int atr, uint64_t ch)
{
    ren.bg_fill = true;
    ren.bg_atr = atr;
    ren.bg_ch = ch;
}

void render_add_model(
        const origin_t * origin,
        const struct model * model,
        size_t iskin
)
{
    if(model == NULL)
    {
        return;
    }

    if(ren.model_list_num >= RENDER_MODEL_LIST_SIZE)
    {
        ERROR("Render buffers owerflow!");
        return;
    }

    struct render_model * rm = &ren.model_list[ren.model_list_num];

    int atr = (iskin == 0 ? 0x1F : 0x44);

    rm->x = origin->x;
    rm->y = origin->y;
    rm->bg_atr = atr;
    rm->model = model;

    ++ren.model_list_num;


}

void render_add_text(int x, int y, int atr, const char * text)
{
    if(ren.text_list_num >= RENDER_TEXT_LIST_SIZE)
    {
        /* render buffers overflow */
        ERROR("Render buffers owerflow!");
        return;
    }

    struct render_text * rt = &ren.text_list[ren.text_list_num];

    rt->x = x;
    rt->y = y;
    rt->bg_atr = atr;
    rt->text = Z_strdup(text);

    ++ren.text_list_num;
}

void render_add_textf(int x, int y, int atr, const char * format, ...)
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

