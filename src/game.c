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
    g_events_init();
    g_ctl_init();
    return 0;
}

void game_done(void)
{
    g_ctl_done();
    text.c.atr=0x0F;
    text.c.chr=0x00;
    text_fill_screen();

    game_directories_done();
}

void game_stop()
{
    g_ctl_game_destroy();
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

void game_handle(const event_t * event)
{
    if(!game.started)
    {
        return;
    }

    switch(event->type)
    {
        case G_EVENT_TICK:
        {
            g_ctl_game_tick();
            break;
        }
        case G_EVENT_KEYBOARD:
        {
            g_ctl_game_input(event->data.KEYBOARD.key);
            break;
        }
        case G_EVENT_STOP_GAME_TICKS:
        {
            game_stop();
            break;
        }
    }
}

void game_draw(void)
{

    if(!game.showmenu)
    {
        g_ctl_scene_draw();
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

    if(game.showmenu)
    {
        menu_handle(event);
    }
    else
    {
        game_handle(event);
    }

}

void game_loop(void)
{
    g_events_pump();
    g_events_handle();
}
