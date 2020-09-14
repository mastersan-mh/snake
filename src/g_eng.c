/**
 * @file g_eng.c
 *
 *  Created on: 7 авг. 2018 г.
 *      Author: mastersan
 */

#include "g_eng.h"

#include "game.h"
#include "world_main.h"
#include "models.h"
#include "hud.h"
#include "render.h"

#include <stdlib.h>

#define FCHECK(xfunc, xres) if((xfunc) == NULL) { return xres; }

static struct gamelib_entrypoint glibctl = {};
static struct game_engine geng = {};

int g_eng_init(void)
{
    geng.ticktime_set = game_ticktime_set;

    geng.game_quit = game_quit;
    geng.event_pump = g_events_event_pump;
    geng.rand = g_events_random;

    geng.render_background = render_background;

    geng.world_find_first_free = world_find_first_free;
    geng.world_ent_unlink = world_ent_unlink;
    geng.world_ent_link = world_ent_link;
    geng.world_ent_update_orig = world_ent_update_orig;
    geng.world_ent_update_model = world_ent_update_model;
    geng.world_ent_update_skin = world_ent_update_skin;
    geng.world_destroy = world_destroy;

    geng.model_precache = model_precache;

    geng.print = hud_print;
    geng.print_centerscreen = hud_print_centerscreen;
    geng.putch = hud_putch;

    glibctl.max_entities = 0;
    gamelib_entrypoint(&glibctl);
    geng.max_entities = glibctl.max_entities;

    FCHECK(glibctl.init, 0);
    return glibctl.init(&geng);
}

void g_eng_done(void)
{
    FCHECK(glibctl.done, );
    glibctl.done();
}

size_t g_eng_entities_max_get(void)
{
    return glibctl.max_entities;
}

void g_eng_tick(void)
{
    FCHECK(glibctl.tick, );
    glibctl.tick();
}

