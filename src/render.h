/*
 * render.h
 *
 *  Created on: 21 авг. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_RENDER_H_
#define SRC_RENDER_H_

#include "vec.h"
#include "models.h"

#include <curses.h>
#include <stdint.h>

#define VID_SCR_WIDTH (80)
#define VID_SCR_HEIGHT (25)

int render_init(void);
void render_done(void);

void render_begin(void);
void render_end(void);

void render_clearbuf(void);
void render(void);

void render_background(int atr, uint64_t ch);

void render_add_model(
        const origin_t * origin,
        const model_t * model,
        size_t iskin
);

void render_add_text(int x, int y, int atr, const char * text);
void render_add_textf(int x, int y, int atr, const char * format, ...);

#endif /* SRC_RENDER_H_ */
