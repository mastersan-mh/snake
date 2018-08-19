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

void snake_init(const game_ctx_t * gctx, const snake_pattern_t * pat);
void snake_done(void);

void ents_game_timing_update(const game_ctx_t * gctx, ent_direction_t direction)
{
    switch(direction)
    {
        case DIRECTION_NORTH:
            gctx->ticktime_set(game_ents.timing * 3 / 2); /* FIXME: 80 / 25 */
            break;
        case DIRECTION_SOUTH:
            gctx->ticktime_set(game_ents.timing * 3 / 2); /* FIXME: 80 / 25 */
            break;
        case DIRECTION_WEST:
            gctx->ticktime_set(game_ents.timing);
            break;
        case DIRECTION_EAST:
            gctx->ticktime_set(game_ents.timing);
            break;
    }
}


static void game_handle_event_tick(const game_ctx_t * gctx)
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
            game_ents.intermission = true;
            break;
        case GSTATE_STOP_LOSE:
            newstate = GSTATE_ENDGAME;
            menu_death_on_enter();
            game_ents.intermission = true;
            break;
        case GSTATE_REQUEST_STOP:
#undef TEXT_ATR
#define TEXT_ATR (0x0F)
            gctx->print_centerscreen(16, TEXT_ATR, "УЖЕ УХОДИШ[Y/N]?");
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
            obj_think(gctx);
            if(snake_is_dead())
            {
                newstate = GSTATE_STOP_LOSE;
            }



            if(!game_ents.paused)
            {
                gamelib_objects_draw(gctx);

            }

            break;
    }

    gamelib_HUD_draw(gctx);

    if(game_ents.paused)
    {
#undef TEXT_ATR
#define TEXT_ATR (0x8F)
        gctx->print_centerscreen(17, TEXT_ATR, "-= P A U S E D =-");
    }

    if(game_ents.intermission)
    {
        gamelib_intermision_draw(gctx);
    }

    game_ents.state = newstate;
}

static void ent_ctrl_game_input(const game_ctx_t * gctx, int key)
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
                    game_ents.state = GSTATE_RUN;
                    break;
                }
            }
            break;
        }
        case GSTATE_ENDGAME:
        {
            bool exit = menu_death_on_event(key);
            menu_death_draw_on_update(gctx);
            if(exit)
            {
                gctx->stop_ticks();
                gctx->show_menu(IMENU_MAIN); /* TODO: move to stop_ticks? */
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
                    ents_game_timing_update(gctx, player_direction());
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
                    ents_game_timing_update(gctx, player_direction());
                    break;
                }
                case IO_KB_UP:
                {
                    player_setdir(DIRECTION_NORTH);
                    ents_game_timing_update(gctx, DIRECTION_NORTH);
                    break;
                }
                case IO_KB_DN:
                {
                    player_setdir(DIRECTION_SOUTH);
                    ents_game_timing_update(gctx, DIRECTION_SOUTH);
                    break;
                }
                case IO_KB_LF:
                {
                    player_setdir(DIRECTION_WEST);
                    ents_game_timing_update(gctx, DIRECTION_WEST);
                    break;
                }
                case IO_KB_RT:
                {
                    player_setdir(DIRECTION_EAST);
                    ents_game_timing_update(gctx, DIRECTION_EAST);
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

static void ent_ctrl_init(const game_ctx_t * gctx)
{
    chart_load();
    game_ents.timing = ENTS_GAME_DEFAULT_TIMING;
    gctx->ticktime_set(game_ents.timing);
}

static void ent_ctrl_done(void)
{
    chart_save();
}

static int ent_ctrl_game_create(int stage, const game_ctx_t * gctx)
{
    if(stage < 0 || 2 < stage)
    {
        return -1;
    }

    game_ents.timing = ENTS_GAME_DEFAULT_TIMING;
    gctx->ticktime_set(game_ents.timing);

    snake_init(gctx, &info_snake[stage]);

    game_ents.state = GSTATE_START;
    game_ents.paused = false;
    game_ents.intermission = false;
    game_ents.timing = 300;
    game_ents.showtiming = 0;

    return 0;
}

static void ent_ctrl_game_destroy(const game_ctx_t * gctx)
{
    obj_freeall();
    snake_done();
    game_ents.timing = ENTS_GAME_DEFAULT_TIMING;
    game_ents.showtiming = 0;
}

static void ent_ctrl_game_tick(const game_ctx_t * gctx)
{
    game_handle_event_tick(gctx);
}

void ent_show_records(const game_ctx_t * gctx)
{
    static const char anti_war[] = "Нет войне! Даешь Rock-N-Roll!";

    size_t row;
    int lev;

#undef TEXT_ATR
#define TEXT_ATR (0x09)
    gctx->print(20, 7, TEXT_ATR, "МЕСТО ИМЯ             ФРАГИ  ВЕС    СТАТУС");

    size_t len = chart_len();

    for(row = 1; row <= len; ++row)
    {
        const chartrec_t *rec = chart_row_get(row - 1);
        lev = rec->scores/SCORES_PER_LEVEL;
        if(lev > LEVEL_MAX - 1)
        {
            lev = LEVEL_MAX - 1;
        }
        gctx->print(20, 7 + row, TEXT_ATR, "%-5d %-15s %-6d %-6d %-20s"
                , (int)row
                , rec->name
                , (int)rec->scores
                , (int)rec->weight
                , (level_str[lev])
        );
    }

#undef TEXT_ATR
#define TEXT_ATR (0x5f)
    gctx->print((80 - 29) / 2, 22, TEXT_ATR, anti_war);
}

void game_ent_ctl_init(game_ctl_t *gctl)
{
    gctl->init = ent_ctrl_init;
    gctl->done = ent_ctrl_done;
    gctl->game_create = ent_ctrl_game_create;
    gctl->game_destroy = ent_ctrl_game_destroy;
    gctl->game_tick = ent_ctrl_game_tick;
    gctl->game_input = ent_ctrl_game_input;
    gctl->show_records = ent_show_records;
}
