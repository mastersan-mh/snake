/*
 * game.c
 *
 *  Created on: 17 дек. 2017 г.
 *      Author: mastersan
 */

#include "io.h"
#include "game.h"
#include "g_types.h"
#include "g_utils.h"
#include "g_events.h"
#include "g_ctl.h"
#include "world_main.h"
#include "models.h"
#include "render.h"

#include "menu.h"

#include "sys_time.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <stdlib.h>

#include <string.h>

static game_t game = {};

int game_init(void)
{
    int res;

    char * home_dir = getenv("HOME");
    if(home_dir == NULL)
    {
        ERROR("Environment variable HOME is NULL")
        return -1;
    }
    game_directories_init(home_dir);

    game.created = false;

    srand(time(NULL));

    res = models_init();
    if(res)
    {
        return res;
    }

    g_events_init();
    res = g_ctl_init();
    if(res) return res;

    size_t max_entities = g_ctl_max_entities_get();
    res = world_init(max_entities);
    if(res)
    {
        g_ctl_done();
        models_done();
        return res;
    }

    io_init();
    res = render_init();
    if(res)
    {
        io_done();
        g_ctl_done();
        models_done();
        return -1;
    }

    return 0;
}

void game_done(void)
{
    render_done();
    io_done();
    world_done();
    g_ctl_done();
    g_events_done();
    models_done();
    game_directories_done();
}

bool game_is_quit(void)
{
    return game.quit;
}

void game_quit(void)
{
    game.quit = true;
}

int game_create(void)
{
    game.created = true;
    return g_ctl_game_create();
}

void game_destroy(void)
{
    g_ctl_game_destroy();
    world_destroy();
    game.created = false;
}

void game_render(void)
{
    render();
}

void game_stop_ticks(void)
{
    g_event_send(G_EVENT_STOP_GAME_TICKS, NULL);
}

void game_ticktime_set(game_time_ms_t ticktime)
{
    g_event_ticktime_set(ticktime);
}

/* game finite-state machine */
void game_event_handle(const event_t * event)
{

    switch(event->type)
    {
        case G_EVENT_VID_WINCH:
        {
            render_winch();
            break;
        }
        case G_EVENT_KEYBOARD:
        {
            g_ctl_game_input(event->data.KEYBOARD.key);
            world_add_to_render();
            break;
        }
        case G_EVENT_TICK:
        {
            render_clearbuf();
            g_ctl_game_tick();
            world_add_to_render();
            break;
        }
        case G_EVENT_STOP_GAME_TICKS:
        {
            /* game_destroy(); */
            break;
        }
    }
}

void game_loop(void)
{
    g_events_pump();
    g_events_handle();
}
