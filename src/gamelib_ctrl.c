/*
 * gamelib_ctrl.c
 *
 *  Created on: 7 авг. 2018 г.
 *      Author: mastersan
 */

#include "gamelib_ctrl.h"

#include "g_eng_exports.h"
#include "gamelib_menu.h"
#include "gamelib_objects.h"
#include "gamelib_chart.h"
#include "gamelib_payer_death.h"
#include "gamelib_common.h"

static void gamelib_game_input(int key);

void ents_game_timing_update(enum direction direction)
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

void gamelib_handle_event_tick(void)
{
    enum g_event_type type;
    struct g_event_data data;

    while(gamelib.geng->event_pump(&type, &data))
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

    if(gamelib.showmenu)
    {
        menu_draw();
    }
    else
    {
        switch(gamelib.state)
        {
            case GSTATE_NONE:
                break;
            case GSTATE_STOP_WIN:
            {
                gamelib.state = GSTATE_ENDGAME;
                menu_death_on_enter();
                gamelib.intermission = true;
                break;
            }
            case GSTATE_STOP_LOSE:
            {
                gamelib.state = GSTATE_ENDGAME;
                menu_death_on_enter();
                gamelib.intermission = true;
                break;
            }
            case GSTATE_REQUEST_STOP:
            {
#undef TEXT_ATR
#define TEXT_ATR (0x0F)
                gamelib.geng->print_centerscreen(16, TEXT_ATR, "УЖЕ УХОДИШЬ[Y/N]?");
                break;
            }
            case GSTATE_ENDGAME:
            {
                /* show your scores an enter your name */
                break;
            }
            case GSTATE_RUN:
            {
                if(gamelib.paused)
                {
                    break;
                }

                if(gamelib.idle_cycle < gamelib.idle_cycles_num)
                {
                    gamelib.idle_cycle++;
                }
                else
                {
                    obj_think();
                    snake_think();
                    if(snake_is_dead())
                    {
                        gamelib.state = GSTATE_STOP_LOSE;
                    }

                    gamelib.idle_cycle = 0;
                }

                break;
            }
        }

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
    else
    {
        switch(gamelib.state)
        {
            case GSTATE_NONE:
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
                    menu_show_menu(IMENU_MAIN);
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
                            gamelib.timing += 10;
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
}

int gamelib_game_create(int stage)
{
    if(stage < 0 || 2 < stage)
    {
        return -1;
    }

    gamelib.timing = ENTS_GAME_DEFAULT_TIMING;
    gamelib.geng->ticktime_set(gamelib.timing);

    snake_init(&info_snake[stage]);

    obj_put(OBJ_MARIJUANA);
    gamelib.state = GSTATE_RUN;

    gamelib.showmenu = false;

    gamelib.paused = false;
    gamelib.intermission = false;
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
