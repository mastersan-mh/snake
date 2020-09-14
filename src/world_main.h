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

/**
 * @brief Destroy the entities of the world
 */
void world_destroy(void);

/**
 * @brief Get index of the first free entity
 * @param[out] ient     Index of free entity
 * @return 0            OK
 * @return < 0          No free entities
 */
int world_find_first_free(world_ientity_t * ient);

/**
 * @brief unlink the entity from world
 */
int world_ent_unlink(world_ientity_t ient);

/**
 * @brief link or relink entity to world
 */
int world_ent_link(world_ientity_t ient);

/**
 * @brief Update the entity (linked or unlinked) origin
 */
int world_ent_update_orig(world_ientity_t ient, const origin_t * origin);

/**
 * @brief Update the entity (linked or unlinked) model
 */
int world_ent_update_model(world_ientity_t ient, size_t imodel);
int world_ent_update_skin(world_ientity_t ient, size_t iskin);
void world_add_to_render(void);

#endif /* SRC_WORLD_MAIN_H_ */
