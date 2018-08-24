/*
 * render.c
 *
 *  Created on: 21 авг. 2018 г.
 *      Author: mastersan
 */

#include "Z_mem.h"

#include "render.h"

#include "io.h"

#include <curses.h>

#include <stdlib.h>

/* screen size by x */
#define TEXT_SCRsx 80
/* screen size by y */
#define TEXT_SCRsy 25

#define BGCOLORS_NUM 8
#define FGCOLORS_NUM 8

#define RENDER_MODEL_LIST_SIZE (80 * 25 + 10)
#define RENDER_TEXT_LIST_SIZE 128

typedef struct
{
    int x;
    int y;
    int atr;
    const model_t * model;
} render_model_t;

typedef struct
{
    int x;
    int y;
    int atr;
    char * text;
} render_text_t;

static render_model_t render_model_list[RENDER_MODEL_LIST_SIZE];
static size_t render_model_list_num = 0;

static render_text_t render_text_list[RENDER_TEXT_LIST_SIZE];
static size_t render_text_list_num = 0;

/**
 * @brief Init the render
 */
void render_init(void)
{
    /* ifdef color */
    start_color();


    //getmaxyx(stdscr, max_y, max_x);

    /* like VGA colors */
    static const int bgcolors[BGCOLORS_NUM] =
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

    static const int fgcolors[FGCOLORS_NUM] =
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
    for(ibgcolor = 0; ibgcolor < BGCOLORS_NUM; ++ibgcolor)
    {
        for(ifgcolor = 0; ifgcolor < FGCOLORS_NUM; ++ifgcolor)
        {
            init_pair(i, fgcolors[ifgcolor], bgcolors[ibgcolor]);
            ++i;
        }
    }
}

/**
 * @brief Done the render
 */
void render_done(void)
{
}


void render_begin(void)
{
#define RENDER_FAST
#ifdef RENDER_FAST
    erase();
#else
    clear();
#endif
}

void render_end(void)
{
    refresh();
}

/*
 * @note
 * atr:
 *   0xBA
 *   B - blink + bgcolor
 *   A - fgcolors
 */
void P_atr_set(uint8_t atr)
{
    int additional_atr = 0;

    uint8_t blink = ((atr & 0x80) >> 4);
    if(blink)
    {
        additional_atr = additional_atr | A_BLINK;
    }
    uint8_t bg    = ((atr & 0x70) >> 4);
    uint8_t fg    = (atr & 0x0F);

    if(fg >= FGCOLORS_NUM)
    {
        fg = fg & 0x07;
        additional_atr = additional_atr | A_BOLD;
    }

    int cpair = bg * FGCOLORS_NUM + fg + 1;

    attron(COLOR_PAIR(cpair) | additional_atr);
};

/**
 * @brief Add a model to a render list
 */
void render_add_model(
        const origin_t * origin,
        const model_t * model,
        size_t iskin
)
{
    if(model == NULL)
    {
        return;
    }

    if(render_model_list_num >= RENDER_MODEL_LIST_SIZE)
    {
        return;
    }

    render_model_t * rm = &render_model_list[render_model_list_num];

    int atr = (iskin == 0 ? 0x1F : 0x44);

    rm->x = origin->x;
    rm->y = origin->y;
    rm->atr = atr;
    rm->model = model;

    ++render_model_list_num;


}

/**
 * @brief Add a text to a text render list
 */
void render_add_text(int x, int y, int atr, const char * text)
{
    if(render_text_list_num >= RENDER_TEXT_LIST_SIZE)
    {
        return;
    }

    render_text_t * rt = &render_text_list[render_text_list_num];

    rt->x = x;
    rt->y = y;
    rt->atr = atr;
    rt->text = Z_strdup(text);

    ++render_text_list_num;
}

/**
 * @brief Add a formatted text to a render
 */
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

void render(void)
{
#define r_print(x, y, format, ...) \
        mvprintw(y, x, format, ##__VA_ARGS__)

    size_t i;

    /* render models */
    for(i = 0; i < render_model_list_num; ++i)
    {
        render_model_t * rm = &render_model_list[i];
        /* simple, print the string :) */
        P_atr_set(rm->atr);
        r_print(rm->x, rm->y + 1, "%s", rm->model->s);
    }
    render_model_list_num = 0;

    /* render text */
    for(i = 0; i < render_text_list_num; ++i)
    {
        render_text_t * rt = &render_text_list[i];
        P_atr_set(rt->atr);

        r_print(rt->x, rt->y, "%s", rt->text);
        Z_free(rt->text);
    }

    render_text_list_num = 0;

    /*
    {
        int i;
        for(i = 0; i <= 8*8; ++i)
        {
            attron(COLOR_PAIR(i));
//            attron(A_NORMAL);
            r_print((i / 8) * 4, i % 8, "%2dt", i);
        }
    }
     */

}



