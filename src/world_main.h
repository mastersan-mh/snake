/*
 * g_ents_main.h
 *
 *  Created on: 19 авг. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_WORLD_MAIN_H_
#define SRC_WORLD_MAIN_H_

#include "world_gamelib.h"

#include <stddef.h>

int world_init(size_t max_entities);
void world_done(void);
void world_destroy(void);
int world_find_first_free(world_ientity_t * ient);
int world_ent_unlink(world_ientity_t ient);
int world_ent_link(world_ientity_t ient);
int world_ent_update_orig(world_ientity_t ient, const origin_t * origin);
int world_ent_update_model(world_ientity_t ient, size_t imodel);
int world_ent_update_skin(world_ientity_t ient, size_t iskin);
void world_add_to_render(void);

#endif /* SRC_WORLD_MAIN_H_ */
