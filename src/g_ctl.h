/*
 * g_ctl.h
 *
 *  Created on: 7 авг. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_G_CTL_H_
#define SRC_G_CTL_H_

#include "g_ctl_lib.h"

void g_ctl_init(void);
void g_ctl_done(void);
int g_ctl_game_create(int stage);
void g_ctl_game_destroy(void);
void g_ctl_game_tick(void);
void g_ctl_game_input(int key);
void g_ctl_show_records(void);

#endif /* SRC_G_CTL_H_ */
