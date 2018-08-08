/*
 * ent_objects.h
 *
 *  Created on: 25 дек. 2017 г.
 *      Author: mastersan
 */

#ifndef SRC_ENT_OBJECTS_H_
#define SRC_ENT_OBJECTS_H_

#include "g_ctl.h"

#define MAP_SX 80
#define MAP_SY 24

#include "__hack.h"

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
    int x,y;
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
    int x,y;
} snake_seg_t;

/**
 * @brief snake
 */
typedef struct
{
    snake_seg_t *H;
    /* прошлое положение хвоста(для стирания следа) */
    int       lastx;
    int       lasty;

    /* направление движения */
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

void ent_scene_draw(game_ctx_t * ctx);

void snake_die(void);
bool snake_is_dead(void);

void player_setdir(ent_direction_t movedir);
ent_direction_t player_direction(void);

int player_scores(void);
const char * player_level(void);
int player_weight(void);

#endif /* SRC_ENT_OBJECTS_H_ */
