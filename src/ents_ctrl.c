/*
 * ents_ctrl.c
 *
 *  Created on: 7 авг. 2018 г.
 *      Author: mastersan
 */

#include "ents_ctrl.h"

#include "g_ctl_lib.h"

#include "ents_objects.h"
#include "ents_chart.h"
#include "ents_payer_death.h"

#define ENTS_GAME_DEFAULT_TIMING 300

game_ents_t game_ents = {};

void snake_init(game_ctx_t * ctx, const snake_pattern_t * pat);
void snake_done(void);

void ents_game_timing_update(game_ctx_t * ctx, ent_direction_t direction)
{
    switch(direction)
    {
        case DIRECTION_NORTH:
            ctx->ticktime_set(game_ents.timing * 3 / 2); /* FIXME: 80 / 25 */
            break;
        case DIRECTION_SOUTH:
            ctx->ticktime_set(game_ents.timing * 3 / 2); /* FIXME: 80 / 25 */
            break;
        case DIRECTION_WEST:
            ctx->ticktime_set(game_ents.timing);
            break;
        case DIRECTION_EAST:
            ctx->ticktime_set(game_ents.timing);
            break;
    }
}


static void game_handle_event_tick(game_ctx_t * ctx)
{
    game_state_t newstate = game_ents.state;

    switch(game_ents.state)
    {
        case GSTATE_START:
            obj_put(OBJ_MARIJUANA);
            newstate = GSTATE_RUN;
            break;
        case GSTATE_STOP_WIN:
            newstate = GSTATE_ENDGAME;
            menu_death_on_enter();
            menu_death_draw_on_enter();
            break;
        case GSTATE_STOP_LOSE:
            newstate = GSTATE_ENDGAME;
            menu_death_on_enter();
            menu_death_draw_on_enter();
            break;
        case GSTATE_REQUEST_STOP:
            break;
        case GSTATE_REQUEST_STOP_CANCEL:
            newstate = GSTATE_RUN;
            break;
        case GSTATE_ENDGAME:
        {
            /* show your scores an enter your name */
            break;
        }
        case GSTATE_RUN:
            if(game_ents.paused)
            {
                break;
            }
            obj_think();
            if(snake_is_dead())
            {
                newstate = GSTATE_STOP_LOSE;
            }
            break;
    }
    game_ents.state = newstate;
}

static void ent_ctrl_game_input(game_ctx_t * ctx, int key)
{
    switch(game_ents.state)
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
                    game_ents.state = GSTATE_STOP_WIN;
                    break;
                }
                case IO_KB_ESC:
                case 'N':
                case 'n':
                {
                    game_ents.state = GSTATE_REQUEST_STOP_CANCEL;
                    break;
                }
            }
            break;
        }
        case GSTATE_REQUEST_STOP_CANCEL:
            break;
        case GSTATE_ENDGAME:
        {
            bool exit = menu_death_on_event(key);
            menu_death_draw_on_update();
            if(exit)
            {
                ctx->stop_ticks();
                ctx->show_menu(IMENU_MAIN); /* TODO: move to stop_ticks? */
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
                    game_ents.paused = !game_ents.paused;
                    break;
                }
                case '=':
                case '+':
                {
                    if(game_ents.timing < 1000)
                    {
                        game_ents.timing +=10;
                    }
                    game_ents.showtiming = 1100 - game_ents.timing;
                    ents_game_timing_update(ctx, player_direction());
                    break;
                }
                case '-':
                case '_':
                {
                    if(game_ents.timing > 10)
                    {
                        game_ents.timing -= 10;
                    }
                    game_ents.showtiming = 1100 - game_ents.timing;
                    ents_game_timing_update(ctx, player_direction());
                    break;
                }
                case IO_KB_UP:
                {
                    player_setdir(DIRECTION_NORTH);
                    ents_game_timing_update(ctx, DIRECTION_NORTH);
                    break;
                }
                case IO_KB_DN:
                {
                    player_setdir(DIRECTION_SOUTH);
                    ents_game_timing_update(ctx, DIRECTION_SOUTH);
                    break;
                }
                case IO_KB_LF:
                {
                    player_setdir(DIRECTION_WEST);
                    ents_game_timing_update(ctx, DIRECTION_WEST);
                    break;
                }
                case IO_KB_RT:
                {
                    player_setdir(DIRECTION_EAST);
                    ents_game_timing_update(ctx, DIRECTION_EAST);
                    break;
                }
                case IO_KB_ESC:
                {
                    game_ents.state = GSTATE_REQUEST_STOP;
                    break;
                }
            }
            break;
        }
    }
}

static void ent_ctrl_init(game_ctx_t * ctx)
{
    chart_load();
    game_ents.timing = ENTS_GAME_DEFAULT_TIMING;
    ctx->ticktime_set(game_ents.timing);
}

static void ent_ctrl_done(void)
{
    chart_save();
}

static int ent_ctrl_game_create(int stage, game_ctx_t * ctx)
{
    if(stage < 0 || 2 < stage)
    {
        return -1;
    }
    snake_init(ctx, &info_snake[stage]);

    game_ents.state = GSTATE_START;
    game_ents.paused = false;
    game_ents.timing = 300;
    game_ents.showtiming = 0;

    return 0;
}

static void ent_ctrl_game_destroy(game_ctx_t * ctx)
{
    obj_freeall();
    snake_done();
    game_ents.timing = ENTS_GAME_DEFAULT_TIMING;
    game_ents.showtiming = 0;
}

static void ent_ctrl_game_tick(game_ctx_t * ctx)
{
    game_handle_event_tick(ctx);
}

void ent_show_records(game_ctx_t * ctx)
{
    static const char anti_war[] = "Нет войне! Даешь Rock-N-Roll!";

    size_t row;
    int lev;

    text.c.atr=0x09;
    text_print(20, 7,"МЕСТО ИМЯ             ФРАГИ  ВЕС    СТАТУС");

    size_t len = chart_len();

    for(row = 1; row <= len; ++row)
    {
        const chartrec_t *rec = chart_row_get(row - 1);
        lev = rec->scores/SCORES_PER_LEVEL;
        if(lev > LEVEL_MAX - 1)
        {
            lev = LEVEL_MAX - 1;
        }
        text_print(20, 7 + row, "%-5d %-15s %-6d %-6d %-20s"
                , (int)row
                , rec->name
                , (int)rec->scores
                , (int)rec->weight
                , (level_str[lev])
        );
    }

    text.c.atr=0x5F;
    text_print((80-29)/2, 22, anti_war);
}

void game_ent_ctl_init(game_ctl_t *ctl)
{
    ctl->init = ent_ctrl_init;
    ctl->done = ent_ctrl_done;
    ctl->game_create = ent_ctrl_game_create;
    ctl->game_destroy = ent_ctrl_game_destroy;
    ctl->game_tick = ent_ctrl_game_tick;
    ctl->game_input = ent_ctrl_game_input;
    ctl->scene_draw = ent_scene_draw;
    ctl->show_records = ent_show_records;
}
