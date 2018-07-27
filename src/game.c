/*
 * game.c
 *
 *  Created on: 17 дек. 2017 г.
 *      Author: mastersan
 */

#include "io.h"
#include "game.h"
#include "g_events.h"

#include "snaketypes.h"
#include "chart.h"
#include "menu.h"

#include "_text.h"
#include "_time.h"
#include "str.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <stdlib.h>

static game_t game = {};

char valid_chars[] =
        "!#$%&'()+,-.0123456789;=@"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`"
        "abcdefghijklmnopqrstuvwxyz{}~"
        "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
        "абвгдежзийклмнопрстуфхцчшщъыьэюяЁё";

//patterns
int pt0[1 * 3] = {1,2,3};
int pt1[7 * 23] = {
         1, 2, 3, 4, 0,30,31, 0, 0,40,41, 0,71,72,73,74,75,76, 77, 78,79,80,81, //**** **  ** ***********
         0, 0, 0, 5, 0,29,32, 0,38,39,42, 0,70, 0, 0, 0, 0, 0, 96, 95, 0, 0,82, //   * ** *** *     **  *
         9, 8, 7, 6, 0,28,33,34,37, 0,43, 0,69,68,67,66,65, 0, 97, 94,93,92,83, //**** **** * ***** *****
        10,11,12, 0, 0,27, 0,35,36, 0,44, 0,60,61,62,63,64, 0, 98, 99, 0,91,84, //***  * ** * ***** ** **
         0, 0,13,14, 0,26, 0, 0, 0, 0,45, 0,59, 0, 0, 0, 0, 0,  0,100, 0,90,85, //  ** *    * *      * **
        18,17,16,15, 0,25, 0, 0, 0, 0,46, 0,58,57,56,55,54, 0,102,101, 0,89,86, //**** *    * ***** ** **
        19,20,21,22,23,24, 0, 0, 0, 0,47,48,49,50,51,52,53, 0,103,  0, 0,88,87  //******    ******* *  **
};

int pt2[5 * 20] =
{
         1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19, 20,
        40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22, 21,
        41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59, 60,
        80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62, 61,
        81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100
};

snake_pattern_t info_snake[] =
{
        {DIRECTION_EAST , 3 , 1, pt0},
        {DIRECTION_SOUTH, 23, 7, pt1},
        {DIRECTION_SOUTH, 20, 5, pt2}
};

char *level[LEVEL_MAX] =
{
        "Так себе, микробик:)",
        "Червячёк            ",
        "Червяк              ",
        "Большой Червяк      ",
        "Глист               ",
        "Взрослый глист      ",
        "Хобот слоненка      ",
        "Хобот слона         ",
        "Матерый бычий цепень",
        "Великий Шланг       ",
        "Бог шлангообразных  "
};



void game_init(void)
{
    game.showmenu = true;

    game.paused = false;
    game.exit_request = false;

    text_init80X25X8();
    text.c.atr=0x00;
    text.c.chr=0x00;
    text_fill_screen();
    srand(time(NULL));
    chart_load();
    g_events_init();
}

void game_done(void)
{
    chart_save();
    text.c.atr=0x0F;
    text.c.chr=0x00;
    text_fill_screen();
    game.showtiming = 0;
    game.timing = 500; //задержка(мс)

}

bool game_is_quit(void)
{
    return game.quit;
}

void game_quit(void)
{
    game.quit = true;
}

void game_start(void)
{
    text.c.atr=0x1F;
    text.c.chr=0x00;
    text_fill_screen();
    game.timing = 500;
    game.state = GSTATE_START;
    game.showmenu = false;
    game.paused = false;
    game.exit_request = false;

}

void game_stop(void)
{
    game.state = GSTATE_NO;
}

void game_delay_update(direction_t direction)
{
    switch(direction)
    {
        case DIRECTION_NORTH:
            g_event_ticktime_set(game.timing * 3 / 2); /* FIXME: 80 / 25 */
            break;
        case DIRECTION_SOUTH:
            g_event_ticktime_set(game.timing * 3 / 2); /* FIXME: 80 / 25 */
            break;
        case DIRECTION_WEST:
            g_event_ticktime_set(game.timing);
            break;
        case DIRECTION_EAST:
            g_event_ticktime_set(game.timing);
            break;
    }
}

void game_snake_die()
{
    game.showmenu = true;
    menu_show_menu(IMENU_DEATH);
}

static void game_handle_event_tick(const event_t * event)
{
    game_state_t newstate = game.state;

    switch(game.state)
    {
        case GSTATE_NO:
            break;
        case GSTATE_START:
            obj_put(OBJ_MARIJUANA);
            newstate = GSTATE_RUN;
            break;
        case GSTATE_RUN:
            obj_think();
            if(snake_is_dead())
            {
                newstate = GSTATE_LOSE;
            }
            break;
        case GSTATE_WIN:
            newstate = GSTATE_STOP;
            break;
        case GSTATE_LOSE:
            game_snake_die();
            newstate = GSTATE_STOP;
            break;
        case GSTATE_STOP:
            obj_freeall();
            snake_done();
            game.showmenu = true;
            break;
    }
    game.state = newstate;
}

static void game_handle_event_keyboard(const event_t * event)
{
    switch(event->data.KEYBOARD.key)
    {
        case 'P':
        case 'p':
        {
            game.paused = !game.paused;
            break;
        }
        case '=':
        case '+':
        {
            if(game.timing < 1000)
            {
                game.timing++;
            }
            game.showtiming = 1100 - game.timing;
            break;
        }
        case '-':
        case '_':
        {
            if(game.timing > 10)
            {
                game.timing--;
            }
            game.showtiming = 1100 - game.timing;
            break;
        }
        case IO_KB_UP:
        {
            player_setdir(DIRECTION_NORTH);
            game_delay_update(DIRECTION_NORTH);
            break;
        }
        case IO_KB_DN:
        {
            player_setdir(DIRECTION_SOUTH);
            game_delay_update(DIRECTION_SOUTH);
            break;
        }
        case IO_KB_LF:
        {
            player_setdir(DIRECTION_WEST);
            game_delay_update(DIRECTION_WEST);
            break;
        }
        case IO_KB_RT:
        {
            player_setdir(DIRECTION_EAST);
            game_delay_update(DIRECTION_EAST);
            break;
        }
        case IO_KB_ESC:
        {
            text.c.atr = 0x0F;
            text_writeATR(30,12,"УЖЕ УХОДИШ[Y/N]?");
            break;
        }
        case 'Y':
        case 'y':
        {
            if(game.exit_request)
            {
                snake_die();
                game_stop();
            }
            break;
        }
        case 'N':
        case 'n':
        {
            if(game.exit_request)
            {
                text.c.atr=0x1F;
                text_writeATR(30,12,"                ");
            }
            break;
        }
    }
}

void game_handle(const event_t * event)
{
    switch(event->type)
    {
        case G_EVENT_TICK:
        {
            game_handle_event_tick(event);
            break;
        }
        case G_EVENT_KEYBOARD:
        {
            game_handle_event_keyboard(event);
            break;
        }
    }
}





static void game_draw_state_run(void)
{
    char str[20];

    text.c.atr=0x0F;
    text_writeATR( 0,0," СОЖРАЛ КОНОПЛИ: ");
    text_writeATR(17,0,str_WORD2strDEC(str,player_scores()));
    text_writeATR(28  ,0,"СТАТУС: ");
    text_writeATR(28+8,0,level[player_level()]);
    text_writeATR(64,0," ВАШ ВЕС: ");
    text_writeATR(74,0,str_WORD2strDEC(str,player_weight()));
    text_writeATR(79,0," ");


    obj_draw();

    if(game.showtiming > 0)
    {
        text_writeATR(0, 24, "timing=");
        text_writeATR(7, 24, str_WORD2strDEC(str,game.timing));
        game.showtiming--;
        if(game.showtiming <= 0)
            text_writeATR(0,24,"            ");
    }
}

void game_draw(void)
{
    static bool paused_prev = false;


    if(game.showmenu)
    {
        menu_draw();
    }
    else
    {
        switch(game.state)
        {

            case GSTATE_NO:
                game.showmenu = true;
                break;
            case GSTATE_START:
                break;
            case GSTATE_RUN:
                game_draw_state_run();
                break;
            case GSTATE_WIN:
                break;
            case GSTATE_LOSE:
                break;
            case GSTATE_STOP:
                break;
        }


        if(game.paused != paused_prev)
        {
            if(game.paused)
            {
                text.c.atr = 0x8F;
                text_writeATR((80-13)/2,12,"-= P A U S E =-");
            }
            else
            {
                text.c.atr = 0x1F;
                text_writeATR((80-13)/2,12,"               ");
            }
        }


    }

    io_render_end();
}

/* game finite-state machine */
void g_fsm(const event_t * event)
{

    if(game.showmenu)
    {
        menu_handle(event);
    }
    else
    {
        game_handle(event);
    }

}

void game_loop(void)
{
    g_events_pump();
    g_events_handle();
}
