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

#define VID_SCR_WIDTH (80)
#define VID_SCR_HEIGHT (25)

void render_add_model(
        const origin_t * origin,
        const model_t * model,
        size_t iskin
);

void render_add_text(int x, int y, int atr, const char * text);

void render(void);

#endif /* SRC_RENDER_H_ */
