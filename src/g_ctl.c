/*
 * g_ctl.c
 *
 *  Created on: 7 авг. 2018 г.
 *      Author: mastersan
 */

#include "g_ctl.h"
#include "game.h"
#include "world_main.h"
#include "models.h"

#include <stdlib.h>

#define FCHECK(xfunc, xres) if((xfunc) == NULL) { return xres; }

static game_ctl_t gctl = {};
static game_ctx_t gctx = {};

void game_ent_ctl_init(game_ctl_t *gctl);

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

int g_ctl_init(void)
{
    gctx.world_find_first_free = world_find_first_free;
    gctx.world_ent_unlink = world_ent_unlink;
    gctx.world_ent_link = world_ent_link;
    gctx.world_ent_update_orig = world_ent_update_orig;
    gctx.world_ent_update_model = world_ent_update_model;
    gctx.world_ent_update_skin = world_ent_update_skin;

    gctx.model_precache = model_precache;

    gctx.stop_ticks = game_stop_ticks;
    gctx.show_menu = game_menu_show;
    gctx.ticktime_set = game_ticktime_set;
    gctx.print = P_print;
    gctx.print_centerscreen = P_print_centerscreen;
    gctx.putch = P_putch;

    gctl.max_entities = 0;
    game_ent_ctl_init(&gctl);
    gctx.max_entities = gctl.max_entities;

    FCHECK(gctl.init, 0);
    return gctl.init(&gctx);
}

void g_ctl_done(void)
{
    FCHECK(gctl.done, );
    gctl.done();
}

size_t g_ctl_max_entities_get(void)
{
    return gctl.max_entities;
}

int g_ctl_game_create(int stage)
{
    FCHECK(gctl.game_create, 0);
    return gctl.game_create(stage);
}

void g_ctl_game_destroy(void)
{
    FCHECK(gctl.game_destroy, );
    gctl.game_destroy();
}

void g_ctl_game_tick(void)
{
    FCHECK(gctl.game_tick, );
    gctl.game_tick();
}

void g_ctl_game_input(int key)
{
    FCHECK(gctl.game_input, );
    gctl.game_input(key);
}

void g_ctl_show_records(void)
{
    FCHECK(gctl.show_records, );
    gctl.show_records();
}
