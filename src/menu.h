/*
 * menu.h
 *
 *  Created on: 17 дек. 2017 г.
 *      Author: mastersan
 */

#ifndef SRC_MENU_H_
#define SRC_MENU_H_

#include "g_events.h"

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
    IMENU_DEATH   ,
} menu_index_t;

void menu_handle(const event_t * event);
void menu_draw(void);

void menu_show_menu(menu_index_t IMENU_DEATH);

#endif /* SRC_MENU_H_ */
