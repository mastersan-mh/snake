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

double map(
        double x,
        double in_min,
        double in_max,
        double out_min,
        double out_max
)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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

    switch(gamelib.state)
    {
        case GSTATE_NONE:
            break;
        case GSTATE_STOP_WIN:
        {
            gamelib_HUD_draw();
            gamelib.state = GSTATE_ENDGAME;
            menu_death_on_enter();
            gamelib.intermission = true;
            break;
        }
        case GSTATE_STOP_LOSE:
        {
            gamelib_HUD_draw();
            gamelib.state = GSTATE_ENDGAME;
            menu_death_on_enter();
            gamelib.intermission = true;
            break;
        }
        case GSTATE_ENDGAME:
        {
            gamelib_HUD_draw();
            /* show your scores an enter your name */
            break;
        }
        case GSTATE_RUN:
        {
            gamelib_HUD_draw();

            if(gamelib.showmenu || gamelib.paused)
            {
                break;
            }

            /* gamelib.idle_cycles_num */
            long idle_cycles_num = map(
                    player_speed(),
                    SNAKE_SPEED_DEFAULT, SNAKE_SPEED_MAX,
                    ILDE_CYCLES, 0
            );
            if(gamelib.idle_cycle < idle_cycles_num)
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

    if(gamelib.intermission)
    {
        gamelib_intermision_draw();
    }

    if(gamelib.paused)
    {
#undef TEXT_ATR
#define TEXT_ATR (0x8F)
        gamelib.geng->print_centerscreen(17, TEXT_ATR, "-= P A U S E D =-");
    }

    if(gamelib.showmenu)
    {
        menu_draw();
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
            case GSTATE_ENDGAME:
            {
                bool exit = menu_death_on_event(key);
                if(exit)
                {
                    gamelib_game_destroy();
                    menu_show_menu(IMENU_MAIN);
                    gamelib.state = GSTATE_NONE;
                    gamelib.intermission = false;
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
                        if(gamelib.paused) break;
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
                        if(gamelib.paused) break;
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
                        if(gamelib.paused) break;
                        player_setdir(DIRECTION_NORTH);
                        ents_game_timing_update(DIRECTION_NORTH);
                        break;
                    }
                    case IO_KB_DN:
                    {
                        if(gamelib.paused) break;
                        player_setdir(DIRECTION_SOUTH);
                        ents_game_timing_update(DIRECTION_SOUTH);
                        break;
                    }
                    case IO_KB_LF:
                    {
                        if(gamelib.paused) break;
                        player_setdir(DIRECTION_WEST);
                        ents_game_timing_update(DIRECTION_WEST);
                        break;
                    }
                    case IO_KB_RT:
                    {
                        if(gamelib.paused) break;
                        player_setdir(DIRECTION_EAST);
                        ents_game_timing_update(DIRECTION_EAST);
                        break;
                    }
                    case IO_KB_ESC:
                    {
                        if(gamelib.paused) break;
                        menu_show_menu(IMENU_MAIN);
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
    int res;
    if(stage < 0 || 2 < stage)
    {
        return -1;
    }

    gamelib.timing = ENTS_GAME_DEFAULT_TIMING;
    gamelib.geng->ticktime_set(gamelib.timing);

    res = snake_init(&info_snake[stage]);
    if(res)
    {
        return res;
    }

    obj_put(OBJ_MARIJUANA);
    gamelib.state = GSTATE_RUN;

    menu_hide_menu();

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
