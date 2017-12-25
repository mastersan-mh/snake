/*
 * game.h
 *
 *  Created on: 17 дек. 2017 г.
 *      Author: mastersan
 */

#ifndef SRC_GAME_H_
#define SRC_GAME_H_

#include "snaketypes.h"
#include "obj.h"

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

void game_events_pump(void);
void game_tick(void);
void game_draw(void);

#endif /* SRC_GAME_H_ */
