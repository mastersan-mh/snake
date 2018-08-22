/*
 * models.c
 *
 *  Created on: 21 авг. 2018 г.
 *      Author: mastersan
 */

#include "models.h"
#include "Z_mem.h"

#define LEN_MAX 5
#define MODELS_MAX (16)
static size_t models_num = 0;

static model_t models[MODELS_MAX];

int models_init(void)
{
    return 0;
}

void models_done(void)
{
    size_t i;
    for(i = 0; i < models_num; ++i)
    {
        Z_free(models[models_num].s);
    }
}

int model_precache(const char * s, size_t * imodel)
{
    if(models_num >= MODELS_MAX)
    {
        return -1;
    }
    models[models_num].s = Z_strndup(s, LEN_MAX);
    *imodel = models_num;
    ++models_num;
    return 0;
}

const model_t * models_model_get(size_t imodel)
{
    if(models_num >= MODELS_MAX)
    {
        return NULL;
    }
    return &models[imodel];
}
