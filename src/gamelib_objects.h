/*
 * gamelib_objects.h
 *
 *  Created on: 25 дек. 2017 г.
 *      Author: mastersan
 */

#ifndef SRC_GAMELIB_OBJECTS_H_
#define SRC_GAMELIB_OBJECTS_H_

#include "g_eng_exports.h"

#define MAP_SX 80
#define MAP_SY 24

#define SCORES_PER_LEVEL 100
#define LEVEL_MAX 11

enum direction
{
    DIRECTION_NORTH,
    DIRECTION_SOUTH,
    DIRECTION_WEST,
    DIRECTION_EAST
};

enum obj_type
{
    /* hemp */
    OBJ_MARIJUANA,
    /* planted hemp */
    OBJ_MARIJUANAP,
    /* Purgen. Oi! Oi! Oi! Punks not dead! */
    OBJ_PURGEN,
    /* shit */
    OBJ_SHIT,
};

/**
 * @brief объекты на карте
 */
struct obj_st
{
    struct obj_st *next;

    world_ientity_t ient;
    origin_t origin;

    enum obj_type type;
    int timer;
};

/**
 * @brief шаблон змеи
 */
struct snake_pattern
{
    char dir;
    int  sx,sy;
    int  *pat;
};

extern struct snake_pattern info_snake[];

extern char *level_str[LEVEL_MAX];

void obj_new(int x, int y, enum obj_type objtype);
void obj_freeall(void);
void obj_free(struct obj_st **obj);

void obj_put(enum obj_type objtype);
void obj_think(void);
void snake_think(void);

void snake_die(void);
bool snake_is_dead(void);

void player_setdir(enum direction movedir);
enum direction player_direction(void);

void gamelib_HUD_draw(void);

int player_scores(void);
const char * player_level(void);
int player_weight(void);

#endif /* SRC_GAMELIB_OBJECTS_H_ */
