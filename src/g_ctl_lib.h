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

/* game context */
typedef struct
{
    /* stop the game cycle */
    void (*stop_ticks)(void);
    void (*show_menu)(menu_index_t imenu);
    void (*ticktime_set)(game_time_ms_t ticktime);
    void (*print)(int x, int y, int atr, const char * format, ...);
    void (*print_centerscreen)(size_t text_width, int atr, const char * format, ...);
    void (*putch)(int x, int y, int atr, char ch);
    int key;
} game_ctx_t;

/* game control */
typedef struct
{
    void (*init)(const game_ctx_t * gctx);
    void (*done)(void);
    int (*game_create)(int stage, const game_ctx_t * gctx);
    void (*game_destroy)(const game_ctx_t * gctx);
    void (*game_tick)(const game_ctx_t * gctx);
    void (*game_input)(const game_ctx_t * gctx, int key);

    void (*show_records)(const game_ctx_t * gctx);

} game_ctl_t;

#endif /* SRC_G_CTL_LIB_H_ */
