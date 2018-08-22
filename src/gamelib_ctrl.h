/*
 * gamelib_ctrl.h
 *
 *  Created on: 8 авг. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_GAMELIB_CTRL_H_
#define SRC_GAMELIB_CTRL_H_

#include "g_ctl_lib.h"

#include "gamelib_objects.h"

typedef enum
{
    MDL_DEFAULT,
    MDL_MARIJUANA ,
    MDL_MARIJUANAP,
    MDL_PURGEN    ,
    MDL_SHIT      ,
    MDL_PLAYER    ,
    MDL_SNAKE_HEAD,
    MDL_SNAKE_TAIL,
    MDL_SNAKE_BODY_VERTICAL  ,
    MDL_SNAKE_BODY_HORIZONTAL,
    MDL_SNAKE_BODY_CORNER_LEFT_UP,
    MDL_SNAKE_BODY_CORNER_RIGHT_UP,
    MDL_SNAKE_BODY_CORNER_LEFT_DOWN,
    MDL_SNAKE_BODY_CORNER_RIGHT_DOWN,
    MDL__NUM
} model_index_t;

typedef enum
{
    GSTATE_START,
    GSTATE_STOP_WIN,
    GSTATE_STOP_LOSE,
    GSTATE_REQUEST_STOP,
    GSTATE_ENDGAME,
    GSTATE_RUN,
} game_state_t;

typedef struct
{
    const game_ctx_t * ctx;
    size_t mdlidx[MDL__NUM];
    game_state_t state;
    bool paused;
    bool intermission;
    long showtiming;
    /* ms */
    game_time_ms_t timing;
} gamelib_t;

extern gamelib_t gamelib;


void ents_game_timing_update(ent_direction_t direction);


#endif /* SRC_GAMELIB_CTRL_H_ */
