/*
 * game.h
 *
 *  Created on: 17 дек. 2017 г.
 *      Author: mastersan
 */

#ifndef SRC_GAME_H_
#define SRC_GAME_H_

#include "snaketypes.h"

extern char valid_chars[];

extern snake_pattern_t info_snake[];
extern char *level[LEVEL_MAX];
extern chart_t chart;
extern snake_t snake;

void snake_init(const snake_pattern_t * pat);
void snake_done(void);

void game_main(void);

void chart_load(void);
void chart_save(void);
int chart_top(chartrec_t *rec);
void chart_newrec(chartrec_t *rec);

#endif /* SRC_GAME_H_ */
