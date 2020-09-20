/*
 * gamelib_common.h
 *
 *  Created on: 13 сент. 2020 г.
 *      Author: mastersan
 */

#ifndef SRC_GAMELIB_COMMON_H_
#define SRC_GAMELIB_COMMON_H_

#define ENTS_GAME_DEFAULT_TIMING  ( 1000 /* msec */ / 25 /* HZ */ )
#define ILDE_CYCLES               (8 - 1)

#define SCORES_PER_LEVEL 100
#define LEVEL_MAX 11

#define SNAKE_SPEED_STEP  10
#define SNAKE_SPEED_DEFAULT       100
#define SNAKE_SPEED_MAX       ((SNAKE_SPEED_DEFAULT) * 2)

#endif /* SRC_GAMELIB_COMMON_H_ */
