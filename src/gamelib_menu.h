/*
 * gamelib_menu.h
 *
 *  Created on: 2 сент. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_GAMELIB_MENU_H_
#define SRC_GAMELIB_MENU_H_

#include "g_ctl_lib.h"

typedef enum
{
    IMENU_NONE    ,
    IMENU_MAIN    ,
    IMENU_NEWGAME0,
    IMENU_NEWGAME1,
    IMENU_NEWGAME2,
    IMENU_CHART   ,
    IMENU_HELP    ,
    IMENU_QUIT    ,
} menu_index_t;

void menu_handle_input(int key);

void menu_handle(void);

void menu_show_menu(menu_index_t imenu);


#endif /* SRC_GAMELIB_MENU_H_ */
