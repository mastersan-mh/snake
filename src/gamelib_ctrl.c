/*
 * gamelib_ctrl.c
 *
 *  Created on: 7 авг. 2018 г.
 *      Author: mastersan
 */

#include "g_ctl_lib.h"

#include "gamelib_ctrl.h"
#include "gamelib_objects.h"
#include "gamelib_chart.h"
#include "gamelib_payer_death.h"

#define ENTS_GAME_DEFAULT_TIMING 300

gamelib_t gamelib = {};

void snake_init(const snake_pattern_t * pat);
void snake_done(void);

void ents_game_timing_update(ent_direction_t direction)
{
    switch(direction)
    {
        case DIRECTION_NORTH:
            gamelib.ctx->ticktime_set(gamelib.timing * 3 / 2); /* FIXME: 80 / 25 */
            break;
        case DIRECTION_SOUTH:
            gamelib.ctx->ticktime_set(gamelib.timing * 3 / 2); /* FIXME: 80 / 25 */
            break;
        case DIRECTION_WEST:
            gamelib.ctx->ticktime_set(gamelib.timing);
            break;
        case DIRECTION_EAST:
            gamelib.ctx->ticktime_set(gamelib.timing);
            break;
    }
}


static void game_handle_event_tick(void)
{
    game_state_t newstate = gamelib.state;

    switch(gamelib.state)
    {
        case GSTATE_START:
            obj_put(OBJ_MARIJUANA);
            newstate = GSTATE_RUN;
            break;
        case GSTATE_STOP_WIN:
            newstate = GSTATE_ENDGAME;
            menu_death_on_enter();
            gamelib.intermission = true;
            break;
        case GSTATE_STOP_LOSE:
            newstate = GSTATE_ENDGAME;
            menu_death_on_enter();
            gamelib.intermission = true;
            break;
        case GSTATE_REQUEST_STOP:
#undef TEXT_ATR
#define TEXT_ATR (0x0F)
            gamelib.ctx->print_centerscreen(16, TEXT_ATR, "УЖЕ УХОДИШ[Y/N]?");
            break;
        case GSTATE_ENDGAME:
        {
            /* show your scores an enter your name */
            break;
        }
        case GSTATE_RUN:
            if(gamelib.paused)
            {
                break;
            }
            obj_think();
            snake_think();
            if(snake_is_dead())
            {
                newstate = GSTATE_STOP_LOSE;
            }

            break;
    }

    gamelib_HUD_draw();

    if(gamelib.paused)
    {
#undef TEXT_ATR
#define TEXT_ATR (0x8F)
        gamelib.ctx->print_centerscreen(17, TEXT_ATR, "-= P A U S E D =-");
    }

    if(gamelib.intermission)
    {
        gamelib_intermision_draw();
    }

    gamelib.state = newstate;
}

static void ent_ctrl_game_input(int key)
{
    switch(gamelib.state)
    {
        case GSTATE_START:
            break;
        case GSTATE_STOP_WIN:
            break;
        case GSTATE_STOP_LOSE:
            break;
        case GSTATE_REQUEST_STOP:
        {
            switch(key)
            {
                case 'Y':
                case 'y':
                {
                    gamelib.state = GSTATE_STOP_WIN;
                    break;
                }
                case IO_KB_ESC:
                case 'N':
                case 'n':
                {
                    gamelib.state = GSTATE_RUN;
                    break;
                }
            }
            break;
        }
        case GSTATE_ENDGAME:
        {
            bool exit = menu_death_on_event(key);
            if(exit)
            {
                gamelib.ctx->stop_ticks();
                gamelib.ctx->show_menu(IMENU_MAIN); /* TODO: move to stop_ticks? */
            }
            break;
        }
        case GSTATE_RUN:
        {
            switch(key)
            {
                case 'P':
                case 'p':
                {
                    gamelib.paused = !gamelib.paused;
                    break;
                }
                case '=':
                case '+':
                {
                    if(gamelib.timing < 1000)
                    {
                        gamelib.timing +=10;
                    }
                    gamelib.showtiming = (1100 - gamelib.timing) / 10;
                    ents_game_timing_update(player_direction());
                    break;
                }
                case '-':
                case '_':
                {
                    if(gamelib.timing > 10)
                    {
                        gamelib.timing -= 10;
                    }
                    gamelib.showtiming = 1100 - gamelib.timing;
                    ents_game_timing_update(player_direction());
                    break;
                }
                case IO_KB_UP:
                {
                    player_setdir(DIRECTION_NORTH);
                    ents_game_timing_update(DIRECTION_NORTH);
                    break;
                }
                case IO_KB_DN:
                {
                    player_setdir(DIRECTION_SOUTH);
                    ents_game_timing_update(DIRECTION_SOUTH);
                    break;
                }
                case IO_KB_LF:
                {
                    player_setdir(DIRECTION_WEST);
                    ents_game_timing_update(DIRECTION_WEST);
                    break;
                }
                case IO_KB_RT:
                {
                    player_setdir(DIRECTION_EAST);
                    ents_game_timing_update(DIRECTION_EAST);
                    break;
                }
                case IO_KB_ESC:
                {
                    gamelib.state = GSTATE_REQUEST_STOP;
                    break;
                }
            }
            break;
        }
    }
}

static int ent_ctrl_init(const game_ctx_t * gctx)
{
    gamelib.ctx = gctx;
    chart_load();
    gamelib.timing = ENTS_GAME_DEFAULT_TIMING;
    gctx->ticktime_set(gamelib.timing);

    gctx->model_precache("?" /* 0x05 */, &gamelib.mdlidx[MDL_DEFAULT]);
    gctx->model_precache("m" /* 0x05 */, &gamelib.mdlidx[MDL_MARIJUANA]);
    gctx->model_precache("M" /* 0x06 */, &gamelib.mdlidx[MDL_MARIJUANAP]);
    gctx->model_precache("P" /* 0x0B */, &gamelib.mdlidx[MDL_PURGEN]);
    gctx->model_precache("@" , &gamelib.mdlidx[MDL_SHIT]);
    gctx->model_precache("0" /* 0x01 */, &gamelib.mdlidx[MDL_SNAKE_HEAD]);
    gctx->model_precache("*" , &gamelib.mdlidx[MDL_SNAKE_TAIL]);
    gctx->model_precache("|" /* 186 */, &gamelib.mdlidx[MDL_SNAKE_BODY_VERTICAL]);
    gctx->model_precache("-" /* 205 */, &gamelib.mdlidx[MDL_SNAKE_BODY_HORIZONTAL]);
    gctx->model_precache("/" /* 201 */, &gamelib.mdlidx[MDL_SNAKE_BODY_CORNER_LEFT_UP]);
    gctx->model_precache("\\" /* 187 */, &gamelib.mdlidx[MDL_SNAKE_BODY_CORNER_RIGHT_UP]);
    gctx->model_precache("L" /* 200 */, &gamelib.mdlidx[MDL_SNAKE_BODY_CORNER_LEFT_DOWN]);
    gctx->model_precache("J" /* 188 */, &gamelib.mdlidx[MDL_SNAKE_BODY_CORNER_RIGHT_DOWN]);

    return 0;
}

static void ent_ctrl_done(void)
{
    chart_save();
}

static int ent_ctrl_game_create(int stage)
{
    if(stage < 0 || 2 < stage)
    {
        return -1;
    }

    gamelib.timing = ENTS_GAME_DEFAULT_TIMING;
    gamelib.ctx->ticktime_set(gamelib.timing);

    snake_init(&info_snake[stage]);

    gamelib.state = GSTATE_START;
    gamelib.paused = false;
    gamelib.intermission = false;
    gamelib.timing = 300;
    gamelib.showtiming = 0;

    return 0;
}

static void ent_ctrl_game_destroy(void)
{
    obj_freeall();
    snake_done();
    gamelib.timing = ENTS_GAME_DEFAULT_TIMING;
    gamelib.showtiming = 0;
}

static void ent_ctrl_game_tick(void)
{
    game_handle_event_tick();
}

void ent_show_records(void)
{
    static const char anti_war[] = "Нет войне! Даешь Rock-N-Roll!";

    size_t row;
    int lev;

#undef TEXT_ATR
#define TEXT_ATR (0x09)
    gamelib.ctx->print(20, 7, TEXT_ATR, "МЕСТО ИМЯ             ФРАГИ  ВЕС    СТАТУС");

    size_t len = chart_len();

    for(row = 1; row <= len; ++row)
    {
        const chartrec_t *rec = chart_row_get(row - 1);
        lev = rec->scores/SCORES_PER_LEVEL;
        if(lev > LEVEL_MAX - 1)
        {
            lev = LEVEL_MAX - 1;
        }
        gamelib.ctx->print(20, 7 + row, TEXT_ATR, "%-5d %-15s %-6d %-6d %-20s"
                , (int)row
                , rec->name
                , (int)rec->scores
                , (int)rec->weight
                , (level_str[lev])
        );
    }

#undef TEXT_ATR
#define TEXT_ATR (0x5f)
    gamelib.ctx->print((80 - 29) / 2, 22, TEXT_ATR, anti_war);
}

void game_ent_ctl_init(game_ctl_t *gctl)
{
    gctl->max_entities = 80 * 25;
    gctl->init = ent_ctrl_init;
    gctl->done = ent_ctrl_done;
    gctl->game_create = ent_ctrl_game_create;
    gctl->game_destroy = ent_ctrl_game_destroy;
    gctl->game_tick = ent_ctrl_game_tick;
    gctl->game_input = ent_ctrl_game_input;
    gctl->show_records = ent_show_records;
}
