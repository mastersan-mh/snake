/*
 * menu.h
 *
 *  Created on: 17 дек. 2017 г.
 *      Author: mastersan
 */

#ifndef SRC_MENU_H_
#define SRC_MENU_H_

#define IMENU_MAIN     0x0000
#define IMENU_NEWGAME0 0x0001
#define IMENU_NEWGAME1 0x0101
#define IMENU_NEWGAME2 0x0201
#define IMENU_CHART    0x0301
#define IMENU_HELP     0x0401
#define IMENU_QUIT     0xFFFF

#define SYS_SPECIAL_LEN (sizeof(sys_special) - 1)

void menu_snake_die(void);

void menu(void);

#endif /* SRC_MENU_H_ */
