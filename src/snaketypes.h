/*
 * snaketypes.h
 *
 *  Created on: 17 дек. 2017 г.
 *      Author: mastersan
 */

#ifndef SRC_SNAKETYPES_H_
#define SRC_SNAKETYPES_H_

#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>

#define LEVEL_MAX 11

#define MAP_SX 80
#define MAP_SY 24

#define SCORES_PER_LEVEL 100

#define KB_BACKSPACE 8
#define KB_ENTER 13
#define KB_ESC   27
#define KB_UP    72
#define KB_DN    80
#define KB_LF    75
#define KB_RT    77

typedef enum
{
    DIRECTION_NORTH,
    DIRECTION_SOUTH,
    DIRECTION_WEST,
    DIRECTION_EAST
} direction_t;

typedef unsigned long DWORD;
typedef long SDWORD;

/**
 * @brief шаблон змеи
 */
typedef struct Ssnakepattern{
    char dir;
    int  sx,sy;
    int  *pat;
} snake_pattern_t;

/**
 * @brief сегмент змеи
 */
typedef struct snake_seg_st{
    struct snake_seg_st *prev;
    struct snake_seg_st *next;
    int x,y;
} snake_seg_t;

/**
 * @brief змея
 */
typedef struct
{
    snake_seg_t *H;
    /* прошлое положение хвоста(для стирания следа) */
    int       lastx;
    int       lasty;

    /* направление движения */
    direction_t movedir;
    int       level;   //уровень развитости
    bool      die;     //умерла?
    long      weight;  //вес змеи
    long      scores;  //очки
} snake_t;

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

#endif /* SRC_SNAKETYPES_H_ */
