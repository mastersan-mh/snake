/*
 * g_lib_ctl.h
 *
 * @note
 *      This file should be used in the game library, and only this file!
 *
 *  Created on: 9 авг. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_G_CTL_LIB_H_
#define SRC_G_CTL_LIB_H_

#include "Z_mem.h"

#include "g_types.h"
#include "io_keys.h"
#include "world_gamelib.h"
#include "sys_utils.h"
#include "sys_time.h"

/* game context */
typedef struct
{
    /* game */
    void (*game_quit)(void);
    int (*game_create)(void);
    void (*game_destroy)(void);

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

    /* models */
    int (*model_precache)(const char * s, size_t * imodel);

    /* stop the game cycle */
    void (*stop_ticks)(void);
    void (*ticktime_set)(game_time_ms_t ticktime);
    void (*print)(int x, int y, int atr, const char * format, ...);
    void (*print_centerscreen)(size_t text_width, int atr, const char * format, ...);
    void (*putch)(int x, int y, int atr, char ch);
    int key;

} game_ctx_t;

/* game control */
typedef struct
{
    /* requested maximum entities */
    size_t max_entities;
    int (*init)(const game_ctx_t * gctx);
    void (*done)(void);
    int (*game_create)(void);
    void (*game_destroy)(void);
    void (*game_tick)(void);
    void (*game_input)(int key);

} game_ctl_t;

#endif /* SRC_G_CTL_LIB_H_ */
