/**
 * @file gamelib_main.c
 *
 *  Created on: 14 сент. 2020 г.
 *      Author: mastersan
 */

#include "g_eng_exports.h"

#include "gamelib_common.h"
#include "gamelib_ctrl.h"
#include "gamelib_menu.h"
#include "gamelib_chart.h"

struct gamelib gamelib = {};

static int gamelib_init(const struct game_engine * geng)
{
    gamelib.geng = geng;
    gamelib.state = GSTATE_NONE;

    menu_show_menu(IMENU_MAIN);

    chart_load();
    gamelib.timing = ENTS_GAME_DEFAULT_TIMING;
    geng->ticktime_set(gamelib.timing);

    geng->model_precache("?" /* 0x05 */, &gamelib.mdlidx[MDL_DEFAULT]);
    geng->model_precache("m" /* 0x05 */, &gamelib.mdlidx[MDL_MARIJUANA]);
    geng->model_precache("M" /* 0x06 */, &gamelib.mdlidx[MDL_MARIJUANAP]);
    geng->model_precache("P" /* 0x0B */, &gamelib.mdlidx[MDL_PURGEN]);
    geng->model_precache("@" , &gamelib.mdlidx[MDL_SHIT]);
    geng->model_precache("0" /* 0x01 */, &gamelib.mdlidx[MDL_SNAKE_HEAD]);
    geng->model_precache("*" , &gamelib.mdlidx[MDL_SNAKE_TAIL]);
    geng->model_precache("|" /* 186 */, &gamelib.mdlidx[MDL_SNAKE_BODY_VERTICAL]);
    geng->model_precache("-" /* 205 */, &gamelib.mdlidx[MDL_SNAKE_BODY_HORIZONTAL]);
    geng->model_precache("/" /* 201 */, &gamelib.mdlidx[MDL_SNAKE_BODY_CORNER_LEFT_UP]);
    geng->model_precache("\\" /* 187 */, &gamelib.mdlidx[MDL_SNAKE_BODY_CORNER_RIGHT_UP]);
    geng->model_precache("L" /* 200 */, &gamelib.mdlidx[MDL_SNAKE_BODY_CORNER_LEFT_DOWN]);
    geng->model_precache("J" /* 188 */, &gamelib.mdlidx[MDL_SNAKE_BODY_CORNER_RIGHT_DOWN]);

    gamelib.idle_cycles_num = ILDE_CYCLES;
    gamelib.idle_cycle = 0;

    return 0;
}

static void gamelib_done(void)
{
    chart_save();
}

static void gamelib_tick(void)
{
    gamelib.geng->render_background(0x1F, ' ');

    gamelib_handle_event_tick();
}

void gamelib_entrypoint(struct gamelib_entrypoint * gamelib_entrypoint)
{
    gamelib_entrypoint->max_entities = 80 * 25;
    gamelib_entrypoint->init = gamelib_init;
    gamelib_entrypoint->done = gamelib_done;
    gamelib_entrypoint->tick = gamelib_tick;
}
