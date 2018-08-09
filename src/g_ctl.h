/*
 * g_ctl.h
 *
 *  Created on: 7 авг. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_G_CTL_H_
#define SRC_G_CTL_H_

#include "g_types.h"
#include "io.h"
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

    int (*player_scores)(void);
    const char * (*player_level)(void);
    int (*player_weight)(void);

    void (*show_records)(game_ctx_t * ctx);

} game_ctl_t;

void g_ctl_init(void);
void g_ctl_done(void);
int g_ctl_game_create(int stage);
void g_ctl_game_destroy(void);
void g_ctl_game_tick(void);
void g_ctl_game_input(int key);
void g_ctl_scene_draw(void);
int g_ctl_player_scores(void);
const char * g_ctl_player_level(void);
int g_ctl_player_weight(void);
void g_ctl_show_records(void);

#endif /* SRC_G_CTL_H_ */
