/**
 * @file g_eng.h
 *
 *  Created on: 7 авг. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_G_ENG_H_
#define SRC_G_ENG_H_

#include "g_eng_exports.h"

int g_eng_init(
        const char * homedir
);
void g_eng_done(void);
size_t g_eng_entities_max_get(void);
void g_eng_tick(void);

#endif /* SRC_G_ENG_H_ */
