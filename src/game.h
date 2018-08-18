/*
 * game.h
 *
 *  Created on: 17 дек. 2017 г.
 *      Author: mastersan
 */

#ifndef SRC_GAME_H_
#define SRC_GAME_H_

#include "sys_time.h"
#include "g_types.h"
#include "g_events.h"
#include "g_ctl.h"

typedef struct
{
    bool quit;
    bool showmenu;
    bool started;

} game_t;

int game_init(void);
void game_done(void);

bool game_is_quit(void);
void game_quit(void);
void game_start(int stage);

void game_loop(void);
void game_draw(void);

void game_stop_ticks(void);
void game_menu_show(menu_index_t imenu);
void game_ticktime_set(game_time_ms_t ticktime);
void game_event_handle(const event_t * event);

#endif /* SRC_GAME_H_ */
