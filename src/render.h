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
#include "eng_curses.h"

#include <stdint.h>

#define VID_SCR_WIDTH (80)
#define VID_SCR_HEIGHT (25)

/**
 * @brief Init the render module
 */
int render_init(void);

/**
 * @brief Destroy the render module
 */
void render_done(void);

void render_begin(void);
void render_end(void);

/**
 * @brief Action on window size changing
 */
void render_winch(void);

/**
 * @brief Clear a render buffers
 */
void render_clearbuf(void);

/**
 * @brief Do render
 */
void render(void);

void render_background(int atr, uint64_t ch);

/**
 * @brief Add a model to a render list
 */
void render_add_model(
        const origin_t * origin,
        const struct model * model,
        size_t iskin
);

/**
 * @brief Add a text to a text render list
 */
void render_add_text(int x, int y, int atr, const char * text);

/**
 * @brief Add a formatted text to a render
 */
void render_add_textf(int x, int y, int atr, const char * format, ...);

#endif /* SRC_RENDER_H_ */
