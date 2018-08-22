/*
 * hud.h
 *
 *  Created on: 22 авг. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_HUD_H_
#define SRC_HUD_H_

#include <stdlib.h>

void hud_print(int x, int y, int atr, const char * format, ...);
void hud_print_centerscreen(size_t text_width, int atr, const char * format, ...);
void hud_putch(int x, int y, int atr, char ch);



#endif /* SRC_HUD_H_ */
