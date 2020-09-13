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

} game_t;

int game_init(void);
void game_done(void);

bool game_is_quit(void);
void game_quit(void);

void game_destroy(void);

void game_loop(void);
void game_render(void);

void game_stop_ticks(void);
void game_ticktime_set(game_time_ms_t ticktime);

#endif /* SRC_GAME_H_ */
