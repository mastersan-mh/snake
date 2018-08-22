/*
 * render.c
 *
 *  Created on: 21 авг. 2018 г.
 *      Author: mastersan
 */
#include "render.h"

#include "_text.h"

#include "Z_mem.h"

#include <stdlib.h>

#define RENDER_MODEL_LIST_SIZE (80 * 25 + 10)
#define RENDER_TEXT_LIST_SIZE 128

typedef struct
{
    int x;
    int y;
    int atr;
    const model_t * model;
} render_model_t;

typedef struct
{
    int x;
    int y;
    int atr;
    char * text;
} render_text_t;

static render_model_t render_model_list[RENDER_MODEL_LIST_SIZE];
static size_t render_model_list_num = 0;

static render_text_t render_text_list[RENDER_TEXT_LIST_SIZE];
static size_t render_text_list_num = 0;

/**
 * @brief Add a model to a render list
 */
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

    if(render_model_list_num >= RENDER_MODEL_LIST_SIZE)
    {
        return;
    }

    render_model_t * rm = &render_model_list[render_model_list_num];

    int atr = (iskin == 0 ? 0x1F : 0x44);

    rm->x = origin->x;
    rm->y = origin->y;
    rm->atr = atr;
    rm->model = model;

    ++render_model_list_num;


}

/**
 * @brief Add a text to a text render list
 */
void render_add_text(int x, int y, int atr, const char * text)
{
    if(render_text_list_num >= RENDER_TEXT_LIST_SIZE)
    {
        return;
    }

    render_text_t * rt = &render_text_list[render_text_list_num];

    rt->x = x;
    rt->y = y;
    rt->atr = atr;
    rt->text = Z_strdup(text);

    ++render_text_list_num;
}

void render(void)
{
    size_t i;

    /* render models */
    for(i = 0; i < render_model_list_num; ++i)
    {
        render_model_t * rm = &render_model_list[i];
        /* simple, print the string :) */
        text.c.atr = rm->atr;
        text_print(rm->x, rm->y + 1, "%s", rm->model->s);
    }
    render_model_list_num = 0;

    /* render text */
    for(i = 0; i < render_text_list_num; ++i)
    {
        render_text_t * rt = &render_text_list[i];
        text.c.atr = rt->atr;
        text_print(rt->x, rt->y, rt->text);
        //        mvwprintw(win, rt->y, rt->x, rt->atr, rt->text);
        Z_free(rt->text);
    }

    render_text_list_num = 0;

}



