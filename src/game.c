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
#include "g_eng.h"
#include "world_main.h"
#include "models.h"
#include "render.h"

#include "gamelib_menu.h"

#include "sys_time.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <stdlib.h>
#include <string.h>

struct game
{
    bool quit;
};

static struct game game = {};

int game_init(void)
{
    int res;

    char * home_dir = getenv("HOME");
    if(home_dir == NULL)
    {
        ERROR("Environment variable HOME is NULL");
        return -1;
    }

    res = models_init();
    if(res)
    {
        return res;
    }

    g_events_init();
    res = g_eng_init(home_dir);
    if(res)
    {
        return res;
    }

    size_t max_entities = g_eng_entities_max_get();
    res = world_init(max_entities);
    if(res)
    {
        g_eng_done();
        models_done();
        return res;
    }

    io_init();
    res = render_init();
    if(res)
    {
        io_done();
        g_eng_done();
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
    g_eng_done();
    g_events_done();
    models_done();
}

bool game_is_quit(void)
{
    return game.quit;
}

void game_quit(void)
{
    game.quit = true;
}

void game_render(void)
{
    render();
}

void game_ticktime_set(game_time_ms_t ticktime)
{
    g_event_ticktime_set(ticktime);
}

void game_loop(void)
{
    g_events_pump();
    g_events_handle();
}
