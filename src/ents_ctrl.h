/*
 * ents_common.h
 *
 *  Created on: 8 авг. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_ENTS_CTRL_H_
#define SRC_ENTS_CTRL_H_

#include "g_ctl_lib.h"

#include "ents_objects.h"

typedef enum
{
    GSTATE_START,
    GSTATE_STOP_WIN,
    GSTATE_STOP_LOSE,
    GSTATE_REQUEST_STOP,
    GSTATE_REQUEST_STOP_CANCEL,
    GSTATE_ENDGAME,
    GSTATE_RUN,
} game_state_t;

typedef struct
{
    game_state_t state;
    bool paused;
    long showtiming;
    /* ms */
    game_time_ms_t timing;
} game_ents_t;

extern game_ents_t game_ents;


void ents_game_timing_update(game_ctx_t * ctx, ent_direction_t direction);


#endif /* SRC_ENTS_CTRL_H_ */
