/*
 * gamelib_ctrl.c
 *
 *  Created on: 7 авг. 2018 г.
 *      Author: mastersan
 */

#include "g_ctl_lib.h"

#include "gamelib_ctrl.h"
#include "gamelib_menu.h"
#include "gamelib_objects.h"
#include "gamelib_chart.h"
#include "gamelib_payer_death.h"
#include "gamelib_common.h"

#define ENTS_GAME_DEFAULT_TIMING 300

gamelib_t gamelib = {};

static void gamelib_game_input(int key);

void gamelib_menu_show(menu_index_t imenu)
{
    menu_show_menu(imenu);
}


void snake_init(const snake_pattern_t * pat);
void snake_done(void);

void ents_game_timing_update(ent_direction_t direction)
{
    switch(direction)
    {
        case DIRECTION_NORTH:
            gamelib.geng->ticktime_set(gamelib.timing * 3 / 2); /* FIXME: 80 / 25 */
            break;
        case DIRECTION_SOUTH:
            gamelib.geng->ticktime_set(gamelib.timing * 3 / 2); /* FIXME: 80 / 25 */
            break;
        case DIRECTION_WEST:
            gamelib.geng->ticktime_set(gamelib.timing);
            break;
        case DIRECTION_EAST:
            gamelib.geng->ticktime_set(gamelib.timing);
            break;
    }
}


static void game_handle_event_tick(void)
{
    enum g_event_type type;
    struct g_event_data data;

    while(gamelib.geng->key_pump(&type, &data))
    {
        switch(type)
        {
            case G_EVENT_KEYBOARD:
            {
                gamelib_game_input(data.KEYBOARD.key);
                break;
            }
        }
    }

    switch(gamelib.state)
    {
        case GSTATE_NONE:
            break;
        case GSTATE_START:
            obj_put(OBJ_MARIJUANA);
            gamelib.state = GSTATE_RUN;
            break;
        case GSTATE_STOP_WIN:
            gamelib.state = GSTATE_ENDGAME;
            menu_death_on_enter();
            gamelib.intermission = true;
            break;
        case GSTATE_STOP_LOSE:
            gamelib.state = GSTATE_ENDGAME;
            menu_death_on_enter();
            gamelib.intermission = true;
            break;
        case GSTATE_REQUEST_STOP:
#undef TEXT_ATR
#define TEXT_ATR (0x0F)
            gamelib.geng->print_centerscreen(16, TEXT_ATR, "УЖЕ УХОДИШ[Y/N]?");
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
                gamelib.state = GSTATE_STOP_LOSE;
            }

            break;
    }

    if(gamelib.showmenu)
    {
        menu_handle();
    }
    else
    {

        gamelib_HUD_draw();

        if(gamelib.paused)
        {
#undef TEXT_ATR
#define TEXT_ATR (0x8F)
            gamelib.geng->print_centerscreen(17, TEXT_ATR, "-= P A U S E D =-");
        }

        if(gamelib.intermission)
        {
            gamelib_intermision_draw();
        }
    }

}

static void gamelib_game_input(int key)
{
    if(gamelib.showmenu)
    {
        menu_handle_input(key);
    }


    switch(gamelib.state)
    {
        case GSTATE_NONE:
            break;
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
                gamelib_game_destroy();
                menu_show_menu(IMENU_MAIN); /* TODO: move to stop_ticks? */
                gamelib.state = GSTATE_NONE;
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

static int gamelib_init(const struct game_engine * geng)
{
    gamelib.geng = geng;
    gamelib.state = GSTATE_NONE;

    gamelib_menu_show(IMENU_MAIN);

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

    return 0;
}

static void gamelib_done(void)
{
    chart_save();
}

int gamelib_game_create(void)
{
    int stage = gamelib.stage;
    if(stage < 0 || 2 < stage)
    {
        return -1;
    }

    gamelib.timing = ENTS_GAME_DEFAULT_TIMING;
    gamelib.geng->ticktime_set(gamelib.timing);

    snake_init(&info_snake[stage]);

    gamelib.state = GSTATE_START;

    gamelib.showmenu = false;

    gamelib.paused = false;
    gamelib.intermission = false;
    gamelib.timing = 300;
    gamelib.showtiming = 0;

    return 0;
}

void gamelib_game_destroy(void)
{
    obj_freeall();
    snake_done();
    gamelib.timing = ENTS_GAME_DEFAULT_TIMING;
    gamelib.showtiming = 0;
    gamelib.geng->world_destroy();
}

static void gamelib_game_tick(void)
{
    gamelib.geng->render_background(0x1F, ' ');

    game_handle_event_tick();
}

void gamelib_show_records(void)
{
    static const char anti_war[] = "Нет войне! Даешь Rock-N-Roll!";

    size_t row;
    int lev;

#undef TEXT_ATR
#define TEXT_ATR (0x09)
    gamelib.geng->print(20, 7, TEXT_ATR, "МЕСТО ИМЯ             ФРАГИ  ВЕС    СТАТУС");

    size_t len = chart_len();

    for(row = 1; row <= len; ++row)
    {
        const chartrec_t *rec = chart_row_get(row - 1);
        lev = rec->scores/SCORES_PER_LEVEL;
        if(lev > LEVEL_MAX - 1)
        {
            lev = LEVEL_MAX - 1;
        }
        gamelib.geng->print(20, 7 + row, TEXT_ATR, "%-5d %-15s %-6d %-6d %-20s"
                , (int)row
                , rec->name
                , (int)rec->scores
                , (int)rec->weight
                , (level_str[lev])
        );
    }

#undef TEXT_ATR
#define TEXT_ATR (0x5f)
    gamelib.geng->print((80 - 29) / 2, 22, TEXT_ATR, anti_war);
}

void game_ent_ctl_init(struct gamelib_ctl *glibctl)
{
    glibctl->max_entities = 80 * 25;
    glibctl->init = gamelib_init;
    glibctl->done = gamelib_done;
    glibctl->game_tick = gamelib_game_tick;
}
