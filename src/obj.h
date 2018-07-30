/*
 * obj.h
 *
 *  Created on: 25 дек. 2017 г.
 *      Author: mastersan
 */

#ifndef SRC_OBJ_H_
#define SRC_OBJ_H_

#include "snaketypes.h"

/* конопля */
#define OBJ_MARIJUANA 0
/* посаженая конопля */
#define OBJ_MARIJUANAP 1
/* слабительное */
#define OBJ_PURGEN 2
/* дерьмо */
#define OBJ_SHIT  3

/**
 * @brief объекты на карте
 */
typedef struct obj_st
{
    struct obj_st *next;
    int x,y;
    int id;
    int timer;
} obj_t;

void obj_new(int x, int y, int id);
void obj_freeall(void);
obj_t *obj_free(obj_t **obj);

void obj_put(int id);
void obj_think(void);
void obj_draw(void);

void snake_die(void);
bool snake_is_dead(void);

void player_setdir(direction_t movedir);

int player_scores(void);
int player_level(void);
int player_weight(void);

#endif /* SRC_OBJ_H_ */
