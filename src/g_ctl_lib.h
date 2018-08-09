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

#include "g_types.h"
#include "io_keys.h"
#include "menu.h"
#include "_text.h"

/* game context */
typedef struct
{
    /* stop the game cycle */
    void (*stop_ticks)(void);
    void (*show_menu)(menu_index_t imenu);
    void (*ticktime_set)(game_time_ms_t ticktime);
    int key;
} game_ctx_t;

/* game control */
typedef struct
{
    void (*init)(game_ctx_t * ctx);
    void (*done)(void);
    int (*game_create)(int stage, game_ctx_t * ctx);
    void (*game_destroy)(game_ctx_t * ctx);
    void (*game_tick)(game_ctx_t * ctx);
    void (*game_input)(game_ctx_t * ctx, int key);
    void (*scene_draw)(game_ctx_t * ctx);

    void (*show_records)(game_ctx_t * ctx);

} game_ctl_t;

#endif /* SRC_G_CTL_LIB_H_ */
