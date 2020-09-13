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

static struct gamelib_ctl glibctl = {};
static struct game_engine geng = {};

void game_ent_ctl_init(struct gamelib_ctl *glibctl);

int g_ctl_init(void)
{
    geng.game_quit = game_quit;
    geng.game_create = game_create;
    geng.game_destroy = game_destroy;

    geng.render_background = render_background;

    geng.world_find_first_free = world_find_first_free;
    geng.world_ent_unlink = world_ent_unlink;
    geng.world_ent_link = world_ent_link;
    geng.world_ent_update_orig = world_ent_update_orig;
    geng.world_ent_update_model = world_ent_update_model;
    geng.world_ent_update_skin = world_ent_update_skin;

    geng.model_precache = model_precache;

    geng.stop_ticks = game_stop_ticks;
    geng.ticktime_set = game_ticktime_set;
    geng.print = hud_print;
    geng.print_centerscreen = hud_print_centerscreen;
    geng.putch = hud_putch;
    geng.key_pump = g_events_event_pump;

    glibctl.max_entities = 0;
    game_ent_ctl_init(&glibctl);
    geng.max_entities = glibctl.max_entities;

    FCHECK(glibctl.init, 0);
    return glibctl.init(&geng);
}

void g_ctl_done(void)
{
    FCHECK(glibctl.done, );
    glibctl.done();
}

size_t g_ctl_max_entities_get(void)
{
    return glibctl.max_entities;
}

int g_ctl_game_create(void)
{
    FCHECK(glibctl.game_create, 0);
    return glibctl.game_create();
}

void g_ctl_game_destroy(void)
{
    FCHECK(glibctl.game_destroy, );
    glibctl.game_destroy();
}

void g_ctl_game_tick(void)
{
    FCHECK(glibctl.game_tick, );
    glibctl.game_tick();
}

