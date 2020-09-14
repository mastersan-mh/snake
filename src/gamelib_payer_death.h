/*
 * gamelib_payer_death.h
 *
 *  Created on: 9 авг. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_GAMELIB_PAYER_DEATH_H_
#define SRC_GAMELIB_PAYER_DEATH_H_

#include <stdbool.h>

void menu_death_on_enter(void);
void gamelib_intermision_draw(void);

bool menu_death_on_event(int key);

#endif /* SRC_GAMELIB_PAYER_DEATH_H_ */
