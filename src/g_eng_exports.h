/*
 * g_lib_ctl.h
 *
 * @note
 *      This file should be used in the game library, and only this file!
 *
 *  Created on: 9 авг. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_G_ENG_EXPORTS_H_
#define SRC_G_ENG_EXPORTS_H_

#include "Z_mem.h"

#include "g_events.h"
#include "g_types.h"
#include "io_keys.h"
#include "world_gamelib.h"
#include "sys_utils.h"
#include "sys_time.h"

/**
 * @brief Game engine functions
 */
struct game_engine
{
    /* game */
    void (*game_quit)(void);

    /* render */
    void (*render_background)(int atr, uint64_t ch);

    /* world */
    /* allocated maximum entities */
    size_t max_entities;
    int (*world_find_first_free)(world_ientity_t * ient);
    int (*world_ent_unlink)(world_ientity_t ient);
    int (*world_ent_link)(world_ientity_t ient);
    int (*world_ent_update_orig)(world_ientity_t ient, const origin_t * origin);
    int (*world_ent_update_model)(world_ientity_t ient, size_t imodel);
    int (*world_ent_update_skin)(world_ientity_t ient, size_t iskin);
    void (*world_destroy)(void);
    /* models */
    int (*model_precache)(const char * s, size_t * imodel);

    /* stop the game cycle */
    void (*ticktime_set)(game_time_ms_t ticktime);
    void (*print)(int x, int y, int atr, const char * format, ...);
    void (*print_centerscreen)(size_t text_width, int atr, const char * format, ...);
    void (*putch)(int x, int y, int atr, char ch);
    bool (*key_pump)(
            enum g_event_type * type,
            struct g_event_data * data
    );

};

/**
 * @brief Game library entry point
 */
struct gamelib_entrypoint
{
    /* requested maximum entities */
    size_t max_entities;
    int (*init)(const struct game_engine * geng);
    void (*done)(void);
    void (*tick)(void);

};

/**
 * @brief The first function called in game library to setup.
 * @note Should be defined in game library.
 * @note Fields of the `gamelib_entrypoint` should be set.
 */
void gamelib_entrypoint(struct gamelib_entrypoint * gamelib_entrypoint);

#endif /* SRC_G_ENG_EXPORTS_H_ */
