/*
 * gamelib_menu.h
 *
 *  Created on: 2 сент. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_GAMELIB_MENU_H_
#define SRC_GAMELIB_MENU_H_

#include "g_eng_exports.h"

enum imenu
{
    IMENU_MAIN    ,
    IMENU_NEWGAME0,
    IMENU_NEWGAME1,
    IMENU_NEWGAME2,
    IMENU_CHART   ,
    IMENU_HELP    ,
    IMENU_QUIT    ,
};

void menu_handle_input(int key);

void menu_draw(void);

void menu_show_menu(enum imenu imenu);


#endif /* SRC_GAMELIB_MENU_H_ */
