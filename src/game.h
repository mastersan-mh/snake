/*
 * game.h
 *
 *  Created on: 17 дек. 2017 г.
 *      Author: mastersan
 */

#ifndef SRC_GAME_H_
#define SRC_GAME_H_

#include "snaketypes.h"
#include "_time.h"
#include "obj.h"

#include "g_events.h"

typedef enum
{
    GSTATE_NO,
    GSTATE_START,
    GSTATE_RUN,
    GSTATE_WIN,
    GSTATE_LOSE,
    GSTATE_STOP
} game_state_t;

typedef struct
{
    bool quit;
    game_state_t state;
    bool showmenu;
    int timing;
    int showtiming;
    int delay;

    game_time_ms_t prev;
    game_time_ms_t now;

    bool paused;
    bool exit_request;

} game_t;

extern char valid_chars[];

extern snake_pattern_t info_snake[];
extern char *level[LEVEL_MAX];

void snake_init(const snake_pattern_t * pat);
void snake_done(void);

void game_init(void);
void game_done(void);

bool game_is_quit(void);
void game_quit(void);
void game_start(void);
void game_stop(void);

void game_delay_update(direction_t direction);

void game_events_pump(void);
void game_loop(void);
void game_draw(void);

void g_fsm(const event_t * event);

#endif /* SRC_GAME_H_ */
