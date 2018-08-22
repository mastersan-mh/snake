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

#include "menu.h"

#include "_text.h"
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

    game.showmenu = true;
    game.started = false;
    text_init80X25X8();
    text.c.atr=0x00;
    text.c.chr=0x00;
    text_fill_screen();
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
    return 0;
}

void game_done(void)
{
    world_done();
    g_ctl_done();
    models_done();

    text.c.atr=0x0F;
    text.c.chr=0x00;
    text_fill_screen();

    game_directories_done();
}

void game_stop()
{
    g_ctl_game_destroy();
    world_destroy();
    game.started = false;
}

bool game_is_quit(void)
{
    return game.quit;
}

void game_quit(void)
{
    game.quit = true;
}

void game_start(int stage)
{
    text.c.atr=0x1F;
    text.c.chr=0x00;
    text_fill_screen();
    game.showmenu = false;
    game.started = true;
    g_ctl_game_create(stage);
}

static void g_scene_draw(void)
{
    world_add_to_render();
}

void game_draw(void)
{

    if(!game.showmenu)
    {
        g_scene_draw();
    }

    io_render_end();
}

void game_stop_ticks(void)
{
    g_event_send(G_EVENT_STOP_GAME_TICKS, NULL);
}

void game_menu_show(menu_index_t imenu)
{
    menu_show_menu(imenu);
    game.showmenu = true;
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
        case G_EVENT_TICK:
        {
            io_render_begin();

            if(game.showmenu)
            {
                menu_handle(event);
            }
            else
            {
                if(game.started)
                {
                    g_ctl_game_tick();
                }
            }
            break;
        }
        case G_EVENT_KEYBOARD:
        {
            if(game.showmenu)
            {
                menu_handle(event);
            }
            else
            {
                if(game.started)
                {
                    g_ctl_game_input(event->data.KEYBOARD.key);
                }
            }
            break;
        }
        case G_EVENT_STOP_GAME_TICKS:
        {
            if(game.started)
            {
                game_stop();
            }
            break;
        }
    }
}

void game_loop(void)
{
    g_events_pump();
    g_events_handle();
}
