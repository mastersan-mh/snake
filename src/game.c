/*
 * game.c
 *
 *  Created on: 17 дек. 2017 г.
 *      Author: mastersan
 */

#include "game.h"

#include "snaketypes.h"
#include "chart.h"
#include "menu.h"

#include "fakefunc.h"
#include "_text.h"
#include "_time.h"
#include "str.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <stdlib.h>
//#include <ncurses.h>



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
    int line_st;
    int line_end;
    int cur_x;
    int cur_y;

    game.showmenu = true;

    text_init80X25X8();
    text.c.atr=0x00;
    text.c.chr=0x00;
    text_fill_screen();
    text_videopage_set(0);
    text_cursor_get(0, &line_st,&line_end,&cur_x,&cur_y);
    text_cursor_setxy(0,80,25);
    srand(time(NULL));
    chart_load();
}

void game_done(void)
{
    int line_st = 0;
    int line_end = 0;
    int cur_x = 0;
    int cur_y = 0;
    chart_save();
    text.c.atr=0x0F;
    text.c.chr=0x00;
    text_fill_screen();
    text_cursor_set(line_st,line_end);
    text_cursor_setxy(0,cur_x,cur_y);
    game.showtiming = 0;
    game.timing = 5; //задержка(мс)

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
    game.timing = 5;
    game.state = GSTATE_START;
}

void game_stop(void)
{
    game.state = GSTATE_NO;
}

void game_events_pump(void)
{
    int key;

    mhtime_delay(game.delay);

    if(kbhit())
    {
        key = getch();
        switch(key)
        {
            case('P'):
            case('p'):
            {
                text.c.atr = 0x8F;
                text_writeATR((80-13)/2,12,"-=P A U S E=-");
                do{
                    key = getch();
                    if(key == 0)
                        getch();
                } while(key != 'P' && key != 'p');
                text.c.atr=0x1F;
                text_writeATR((80-13)/2,12,"             ");
                break;
            }
            case('='):
            case('+'):
            {
                if(game.timing<100)
                {
                    game.timing++;
                }
                game.showtiming = 110 - game.timing;

                break;
            }
            case('-'):
            case('_'):
            {
                if(game.timing>0)
                {
                    game.timing--;
                }
                game.showtiming = 110 - game.timing;
                break;
            }
            case 0x00 :
            {
                key = getch();
                switch(key){
                    case KB_UP:
                        player_setdir(DIRECTION_NORTH);
                        game.delay = game.timing*3/2; /* FIXME: 80 / 25 */
                        break;
                    case KB_DN:
                        player_setdir(DIRECTION_SOUTH);
                        game.delay = game.timing*3/2; /* FIXME: 80 / 25 */
                        break;
                    case KB_LF:
                        player_setdir(DIRECTION_WEST);
                        game.delay = game.timing;
                        break;
                    case KB_RT:
                        player_setdir(DIRECTION_EAST);
                        game.delay = game.timing;
                        break;
                }
                break;

            }
            case(KB_ESC):
            {
                text.c.atr = 0x0F;
                text_writeATR(30,12,"УЖЕ УХОДИШ[Y/N]");
                do{
                    key = getch();
                    if(!key) getch();
                }while(!key);
                if(key=='Y' || key=='y')
                { /* клавиша 'Y' */
                    snake_die();
                    game_stop();
                }
                else
                {
                    text.c.atr=0x1F;
                    text_writeATR(30,12,"                ");
                }
                break;
            }
        }
    }
}

void game_fsm(void)
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
            menu_snake_die();
            newstate = GSTATE_STOP;
            break;
        case GSTATE_STOP:
            obj_freeall();
            snake_done();
            break;
    }
    game.state = newstate;
}

/////////////////////////////////////////////////
//игра
/////////////////////////////////////////////////
void game_tick(void)
{
    if(game.showmenu)
    {
        menu();
    }
    else
    {
        game_fsm();
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
    if(game.showmenu)
    {
    }
    else
    {
        switch(game.state)
        {

            case GSTATE_NO:
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

    }
}
