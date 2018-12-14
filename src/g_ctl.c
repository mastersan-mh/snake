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
#include "hud.h"
#include "render.h"

#include <stdlib.h>

#define FCHECK(xfunc, xres) if((xfunc) == NULL) { return xres; }

static game_ctl_t gctl = {};
static game_ctx_t gctx = {};

void game_ent_ctl_init(game_ctl_t *gctl);

int g_ctl_init(void)
{
    gctx.game_quit = game_quit;
    gctx.game_create = game_create;
    gctx.game_destroy = game_destroy;

    gctx.render_background = render_background;

    gctx.world_find_first_free = world_find_first_free;
    gctx.world_ent_unlink = world_ent_unlink;
    gctx.world_ent_link = world_ent_link;
    gctx.world_ent_update_orig = world_ent_update_orig;
    gctx.world_ent_update_model = world_ent_update_model;
    gctx.world_ent_update_skin = world_ent_update_skin;

    gctx.model_precache = model_precache;

    gctx.stop_ticks = game_stop_ticks;
    gctx.ticktime_set = game_ticktime_set;
    gctx.print = hud_print;
    gctx.print_centerscreen = hud_print_centerscreen;
    gctx.putch = hud_putch;

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

int g_ctl_game_create(void)
{
    FCHECK(gctl.game_create, 0);
    return gctl.game_create();
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
