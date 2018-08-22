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

void render_add_model(
        const origin_t * origin,
        const model_t * model,
        size_t iskin
);

#endif /* SRC_RENDER_H_ */
