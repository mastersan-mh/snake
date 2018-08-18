/*
 * g_ctl.c
 *
 *  Created on: 7 авг. 2018 г.
 *      Author: mastersan
 */

#include "g_ctl.h"
#include "game.h"

#include <stdlib.h>

#define FCHECK(xfunc, xres) if((xfunc) == NULL) { return xres; }

static game_ctl_t g_ctl = {};
static game_ctx_t g_ctx = {};

void game_ent_ctl_init(game_ctl_t *ctl);

#include "_text.h"
static void P_print(int x, int y, int atr, const char * format, ...)
{
    va_list args;
    WINDOW * win = stdscr;

    mvwprintw(win, y, x, "");
    va_start(args, format);
    vwprintw(win, format, args);
    va_end(args);
}


#define VID_SCR_WIDTH (80)
#define VID_SCR_HEIGHT (25)
static void P_print_centerscreen(size_t text_width, int atr, const char * format, ...)
{
    va_list args;
    WINDOW * win = stdscr;

    mvwprintw(win, (VID_SCR_HEIGHT / 2), (VID_SCR_WIDTH - text_width) / 2, "");
    va_start(args, format);
    vwprintw(win, format, args);
    va_end(args);


}

static void P_putch(int x, int y, int atr, char ch)
{
    text.c.atr = atr;
    text.c.chr = ch;
    text_setch(x, y);

}

void g_ctl_init(void)
{
    g_ctx.stop_ticks = game_stop_ticks;
    g_ctx.show_menu = game_menu_show;
    g_ctx.ticktime_set = game_ticktime_set;
    g_ctx.print = P_print;
    g_ctx.print_centerscreen = P_print_centerscreen;
    g_ctx.putch = P_putch;

    game_ent_ctl_init(&g_ctl);
    FCHECK(g_ctl.init, );
    g_ctl.init(&g_ctx);
}

void g_ctl_done(void)
{
    FCHECK(g_ctl.done, );
    g_ctl.done();
}

int g_ctl_game_create(int stage)
{
    FCHECK(g_ctl.game_create, 0);
    return g_ctl.game_create(stage, &g_ctx);
}

void g_ctl_game_destroy(void)
{
    FCHECK(g_ctl.game_destroy, );
    g_ctl.game_destroy(&g_ctx);
}

void g_ctl_game_tick(void)
{
    FCHECK(g_ctl.game_tick, );
    g_ctl.game_tick(&g_ctx);
}

void g_ctl_game_input(int key)
{
    FCHECK(g_ctl.game_input, );
    g_ctl.game_input(&g_ctx, key);
}

void g_ctl_scene_draw(void)
{
    FCHECK(g_ctl.scene_draw, );
    g_ctl.scene_draw(&g_ctx);
}

void g_ctl_show_records(void)
{
    FCHECK(g_ctl.show_records, );
    g_ctl.show_records(&g_ctx);
}
