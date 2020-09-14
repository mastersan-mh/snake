/*
 * models.h
 *
 *  Created on: 21 авг. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_MODELS_H_
#define SRC_MODELS_H_

#include <stddef.h>

struct model
{
    char * s;
};

int models_init(void);
void models_done(void);
int model_precache(const char * s, size_t * imodel);
const struct model * models_model_get(size_t imodel);

#endif /* SRC_MODELS_H_ */
