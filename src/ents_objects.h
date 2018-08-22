/*
 * ent_objects.h
 *
 *  Created on: 25 дек. 2017 г.
 *      Author: mastersan
 */

#ifndef SRC_ENT_OBJECTS_H_
#define SRC_ENT_OBJECTS_H_

#include "g_ctl_lib.h"

#define MAP_SX 80
#define MAP_SY 24

#define SCORES_PER_LEVEL 100
#define LEVEL_MAX 11

typedef enum
{
    DIRECTION_NORTH,
    DIRECTION_SOUTH,
    DIRECTION_WEST,
    DIRECTION_EAST
} ent_direction_t;

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

    world_ientity_t ient;
    origin_t origin;

    obj_type_t type;
    int timer;
} obj_t;

/**
 * @brief шаблон змеи
 */
typedef struct
{
    char dir;
    int  sx,sy;
    int  *pat;
} snake_pattern_t;

/**
 * @brief сегмент змеи
 */
typedef struct snake_seg_st
{
    struct snake_seg_st *prev;
    struct snake_seg_st *next;

    world_ientity_t ient;
    origin_t origin;
} snake_seg_t;

/**
 * @brief snake
 */
typedef struct
{
    snake_seg_t *head;

    /* movement direction */
    ent_direction_t movedir;
    int       level;   //уровень развитости
    bool      dead;     //умерла?
    long      weight;  //вес змеи
    long      scores;  //очки
} snake_t;

extern snake_pattern_t info_snake[];


extern char *level_str[LEVEL_MAX];

void obj_new(int x, int y, obj_type_t id);
void obj_freeall(void);
obj_t *obj_free(obj_t **obj);

void obj_put(obj_type_t id);
void obj_think(void);
void snake_think(void);

void snake_die(void);
bool snake_is_dead(void);

void player_setdir(ent_direction_t movedir);
ent_direction_t player_direction(void);

void gamelib_HUD_draw(void);

int player_scores(void);
const char * player_level(void);
int player_weight(void);

#endif /* SRC_ENT_OBJECTS_H_ */
