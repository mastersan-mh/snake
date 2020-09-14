/*
 * g_ents_main.c
 *
 *  Created on: 19 авг. 2018 г.
 *      Author: mastersan
 */


#include "Z_mem.h"
#include "world_gamelib.h"
#include "render.h"

#include "g_types.h"
#include <stdlib.h>

#define PROTECT_IENT(xient, xres) if((xient) > entities_num) return xres;

typedef struct
{
    bool used;
    origin_t orig;
    origin_t orig_prev;
    size_t imodel;
    size_t iskin;
} entity_t;

/* Maximum number of used entity */
static world_ientity_t entities_max = 0;
/* Amount of reserved entities */
static world_ientity_t entities_num = 0;
/* Array of entities */
static entity_t * entities = NULL;

static void P_entities_map_update(world_ientity_t ient)
{
    world_ientity_t ient_num = ient + 1;
    if(entities_max < ient_num)
    {
        entities_max = ient_num;
    }
}

int world_init(size_t max_entities)
{
    entities = Z_calloc(max_entities, sizeof(entity_t));
    if(entities == NULL)
    {
        return -1;
    }
    entities_num = max_entities;
    return 0;
}

void world_done(void)
{
    if(entities == NULL)
    {
        return;
    }
    Z_free(entities);
}

void world_destroy(void)
{
    size_t i;
    for(i = 0; i < entities_max; ++i)
    {
        entity_t * ent = &entities[i];
        ent->used = false;
    }
}

int world_find_first_free(world_ientity_t * ient)
{
    size_t i;
    for(i = 0; i < entities_num; ++i)
    {
        entity_t * ent = &entities[i];
        if(!ent->used)
        {
            *ient = i;
            return 0;
        }
    }
    return -1;
}

int world_ent_unlink(world_ientity_t ient)
{
    PROTECT_IENT(ient, -1);
    entity_t * ent = &entities[ient];
    ent->used = false;
    return 0;
}

int world_ent_link(world_ientity_t ient)
{
    PROTECT_IENT(ient, -1);

    P_entities_map_update(ient);

    entity_t * ent = &entities[ient];
    ent->used = true;
    return 0;
}

int world_ent_update_orig(world_ientity_t ient, const origin_t * origin)
{
    PROTECT_IENT(ient, -1);

    entity_t * ent = &entities[ient];
    if(!ent->used)
    {
        ent->orig_prev = *origin;
    }
    else
    {
        ent->orig_prev = ent->orig;
    }
    ent->orig = *origin;
    return 0;
}

int world_ent_update_model(world_ientity_t ient, size_t imodel)
{
    PROTECT_IENT(ient, -1);

    entity_t * ent = &entities[ient];
    ent->imodel = imodel;
    return 0;
}

int world_ent_update_skin(world_ientity_t ient, size_t iskin)
{
    PROTECT_IENT(ient, -1);

    entity_t * ent = &entities[ient];
    ent->iskin = iskin;
    return 0;
}

void world_add_to_render(void)
{
    size_t i;
    for(i = 0; i < entities_max; ++i)
    {
        entity_t * ent = &entities[i];
        if(!ent->used)
        {
            continue;
        }

        render_add_model(
                &ent->orig,
                models_model_get(ent->imodel),
                ent->iskin
        );

    }
}
