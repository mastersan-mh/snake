/*
 * render.c
 *
 *  Created on: 21 авг. 2018 г.
 *      Author: mastersan
 */
#include "render.h"

#include "_text.h"

void render_add_model(
        const origin_t * origin,
        const model_t * model,
        size_t iskin
)
{
    if(model == NULL)
    {
        return;
    }

    /* simple, print the string :) */

    int atr = (iskin == 0 ? 0x1F : 0x44);

    text.c.atr = atr;
    text_print(origin->x, origin->y + 1, "%s", model->s);

}
