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
#include "obj.h"

#include "g_events.h"

typedef enum
{
    GSTATE_START,
    GSTATE_STOP_WIN,
    GSTATE_STOP_LOSE,
    GSTATE_REQUEST_STOP,
    GSTATE_REQUEST_STOP_CANCEL,
    GSTATE_RUN,
} game_state_t;

typedef struct
{
    bool quit;
    game_state_t state;
    bool showmenu;
    int timing;
    int showtiming;
    int delay;

    bool paused;
    bool exit_request;

} game_t;

extern char valid_chars[];

extern snake_pattern_t info_snake[];
extern char *level_str[LEVEL_MAX];

void snake_init(const snake_pattern_t * pat);
void snake_done(void);

int game_init(void);
void game_done(void);

bool game_is_quit(void);
void game_quit(void);
void game_start(void);

void game_timing_update(direction_t direction);

void game_loop(void);
void game_draw(void);

void g_event_handle(const event_t * event);

#endif /* SRC_GAME_H_ */
