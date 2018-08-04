/*
 * obj.h
 *
 *  Created on: 25 дек. 2017 г.
 *      Author: mastersan
 */

#ifndef SRC_OBJ_H_
#define SRC_OBJ_H_

#include "snaketypes.h"

typedef enum
{
    /* hemp */
    OBJ_MARIJUANA,
    /* planted hemp */
    OBJ_MARIJUANAP,
    /* Purgen. Oi! Oi! Oi! Punks not dead! */
    OBJ_PURGEN,
    /* shit */
    OBJ_SHIT,
    /* player */
    OBJ_PLAYER,
} obj_type_t;

/**
 * @brief объекты на карте
 */
typedef struct obj_st
{
    struct obj_st *next;
    int x,y;
    obj_type_t type;
    int timer;
} obj_t;

void obj_new(int x, int y, obj_type_t id);
void obj_freeall(void);
obj_t *obj_free(obj_t **obj);

void obj_put(obj_type_t id);
void obj_think(void);
void obj_draw(void);

void snake_die(void);
bool snake_is_dead(void);

void player_setdir(direction_t movedir);
direction_t player_direction(void);

int player_scores(void);
int player_level(void);
int player_weight(void);

#endif /* SRC_OBJ_H_ */
