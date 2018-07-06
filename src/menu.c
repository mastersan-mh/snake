/*
 * menu.c
 */

#include "io.h"
#include "snaketypes.h"
#include "menu.h"
#include "game.h"
#include "chart.h"

#include "fakefunc.h"
#include "_text.h"
#include "str.h"

char sys_progversion[] = "SNAKE ver 1.55 (modif: 03.05.2007 ,create(v0.1b): 25.03.2004)";
char sys_special    [] = "Здесь никогда не будет вашей рекламы";

char anti_war[] = "Нет войне! Даешь Rock-N-Roll!";

/////////////////////////////////////////////////
//главное меню
/////////////////////////////////////////////////
int menu_main()
{
    int key;
    int sub=0;
    text.c.atr=0x00;
    text.c.chr=0x00;
    text_fill_screen();

    text.c.atr=0x09;
    text_writeATR(29,1,"**** **  ** ***********");
    text_writeATR(29,2,"   * ** *** *     **  *");
    text_writeATR(29,3,"**** **** * ***** *****");
    text_writeATR(29,4,"***  * ** * ***** ** **");
    text_writeATR(29,5,"  ** *    * *      * **");
    text_writeATR(29,6,"**** *    * ***** ** **");
    text_writeATR(29,7,"******    ******* *  **");

    text_writeATR(40-(SYS_SPECIAL_LEN / 2), 9, sys_special);

    text_writeATR(1,24,sys_progversion);
    text.c.atr=0x5F;
    text_writeATR((80-10)/2,12," НОВАЯ 1  ");
    text_writeATR((80-10)/2,13," НОВАЯ 2  ");
    text_writeATR((80-10)/2,14," НОВАЯ 3  ");
    text_writeATR((80-10)/2,15,"ПОБЕДИТЕЛИ");
    text_writeATR((80-10)/2,16,"  ПОМОЩЬ  ");
    text_writeATR((80-10)/2,17,"  ВЫХОД   ");
    text.c.atr=0x0F;
    text_writeATR( 7,22,"Mad House Software");
    text_writeATR(10,23,"Programming: Ремнёв Александр a.k.a. MasterSan[MH]");

    for(;;)
    {
        text.c.atr=0x05;
        text_writeATR((80-10)/2-2,12+sub,"->");
        text_writeATR((80-10)/2+10,12+sub,"<-");
        key=io_getch();
        text_writeATR((80-10)/2-2,12+sub,"  ");
        text_writeATR((80-10)/2+10,12+sub,"  ");

        switch(key)
        {
            case(IO_KB_UP):
            {
                if(sub>0) sub--; else sub=5;
                break;
            }
            case(IO_KB_DN):
            {
                if(sub<5) sub++; else sub=0;
                break;
            }
            case(IO_KB_ENTER):
            {
                switch(sub)
                {
                    case 0:return(IMENU_NEWGAME0);
                    case 1:return(IMENU_NEWGAME1);
                    case 2:return(IMENU_NEWGAME2);
                    case 3:return(IMENU_CHART);
                    case 4:return(IMENU_HELP);
                    case 5:return(IMENU_QUIT);
                }
                break;
            }
        }
    }
    return IMENU_MAIN;
}

/////////////////////////////////////////////////
//новая игра 0
/////////////////////////////////////////////////
int menu_newgame0(void)
{
    game_start();
    snake_init(&info_snake[0]);
    return(IMENU_MAIN);
}

/////////////////////////////////////////////////
//новая игра 1
/////////////////////////////////////////////////
int menu_newgame1(void)
{
    snake_init(&info_snake[1]);
    game_start();
    return(IMENU_MAIN);
}

/////////////////////////////////////////////////
//новая игра 2
/////////////////////////////////////////////////
int menu_newgame2(void)
{
    snake_init(&info_snake[2]);
    game_start();
    return(IMENU_MAIN);
}

int menu_chart(void)
{
    char str[12];
    size_t row;
    int lev;
    text.c.atr=0x00;
    text.c.chr=0x00;
    text_fill_screen();
    text.c.atr=0x09;
    text_writeATR(20+ 1,7+0,"МЕСТО ИМЯ             ФРАГИ ВЕС   СТАТУС");

    size_t len = chart_len();

    for(row = 1; row <= len; ++row)
    {
        const chartrec_t *rec = chart_row_get(row - 1);
        lev = rec->scores/SCORES_PER_LEVEL;
        if(lev > LEVEL_MAX - 1)
            lev = LEVEL_MAX - 1;


        text_writeATR(20 +  1, 7 + row, str_WORD2strDEC(str, row));
        text_writeATR(20 +  7, 7 + row, rec->name);
        text_writeATR(20 + 23, 7 + row, str_WORD2strDEC(str,rec->scores));
        text_writeATR(20 + 29, 7 + row, str_WORD2strDEC(str,rec->weight));
        text_writeATR(20 + 35, 7 + row, level[lev]);
    }

    text.c.atr=0x5F;
    text_writeATR((80-29)/2,22,anti_war);
    text.c.atr=0x8F;
    text_writeATR((80-16)/2,23,"PRESS ANY KEY...");
    io_getch();
    return(IMENU_MAIN);
}

/////////////////////////////////////////////////
//помощь
/////////////////////////////////////////////////
int menu_help(void)
{
    text.c.atr=0x00;
    text.c.chr=0x00;
    text_fill_screen();
    text.c.atr=0x09;

    text_writeATR((80-13)/2,5   ,"ТИПА ЭТО ХЕЛП");
    text_writeATR(20,7+ 0,"Управление:");
    text_writeATR(20,7+ 1,"Стрелки - Указание направления движения");
    text_writeATR(20,7+ 2,"+,-     - Управление скоростью игры");
    text_writeATR(20,7+ 3,"P       - Пауза");
    text_writeATR(20,7+ 4,"ESC     - Выход в меню");
    text_writeATR(20,7+ 6,"Предметы:");
    text_writeATR(20,7+ 7,"\5 - конопля");
    text_writeATR(20,7+ 8,"\6 - выросшая конопля");
    text_writeATR(20,7+ 9,"\13 - пурген");
    text_writeATR(20,7+10,"@ - дерьмо");

    text.c.atr=0x8F;
    text_writeATR((80-16)/2,23,"PRESS ANY KEY...");

    io_getch();

    return(IMENU_MAIN);
}

//////////////////////////////////////////////////
//меню "змеиной смерти"
//////////////////////////////////////////////////
void menu_snake_die(void)
{
    int key;
    int quit=0;
    int c;
    int count;
    char str[6];
    chartrec_t rec;
    text.c.atr=0x0F;
    text_writeATR(32, 3,"Tы типа сдох :-(");
    text.c.atr=0x2F;
    text_writeATR(30, 5,"  ****************  ");
    text_writeATR(30, 6," *                * ");
    text_writeATR(30, 7,"*    **      **    *");
    text_writeATR(30, 8,"*    **      **    *");
    text_writeATR(30, 9,"*        **        *");
    text_writeATR(30,10,"*        **        *");
    text_writeATR(30,11,"*        **        *");
    text_writeATR(30,12,"*        **        *");
    text_writeATR(30,13,"*                  *");
    text_writeATR(30,14,"*     ********     *");
    text_writeATR(30,15,"*   **        **   *");
    text_writeATR(30,16,"*                  *");
    text_writeATR(30,17," **              ** ");
    text_writeATR(30,18,"   **************   ");
    text_writeATR(26,20,"СОЖРАЛ КОНОПЛИ(КГ): ");
    text_writeATR(26+20,20,str_WORD2strDEC(str,player_scores()));

    rec.weight=player_weight();
    rec.scores=player_scores();
    rec.name[0]=0x00;

    if(!chart_in_chart(&rec)){//не попали в 10 лучших
        text_writeATR(35,21,"ТЫ ХУДШИЙ!");
        io_getch();
        return;
    }

    text_writeATR(26,21,"ИМЯ>");

    count=0;
    while(!quit)
    {
        text.c.atr=0x0F;
        c = 0;
        while(c<count && rec.name[c]){
            text.c.chr = rec.name[c];
            text_setch(30+c,21);
            c++;
        }
        text.c.chr = 176;
        while(c<16)
        {
            text_setch(30+c,21);
            c++;
        }

        key = io_getch();
        switch(key)
        {
            case IO_KB_BACKSPACE:
            {
                if(count>0){
                    rec.name[count]=0x00;
                    --count;
                }
                break;
            }
            case IO_KB_ENTER:
            {
                chart_insert(&rec);
                quit=1;
                break;
            }
            default:
            {
                if(count<15 && str_char_find1st(valid_chars, key)!=-1)
                {
                    rec.name[count] = key;
                    ++count;
                    rec.name[count] = 0x00;
                }
                break;
            }
        }
    }
}

static int Imenu = IMENU_MAIN;

void menu(void)
{
    switch(Imenu)
    {
        case(IMENU_MAIN    ): Imenu = menu_main();break;
        case(IMENU_NEWGAME0): Imenu = menu_newgame0();break;
        case(IMENU_NEWGAME1): Imenu = menu_newgame1();break;
        case(IMENU_NEWGAME2): Imenu = menu_newgame2();break;
        case(IMENU_CHART   ): Imenu = menu_chart();break;
        case(IMENU_HELP    ): Imenu = menu_help();break;
        case(IMENU_QUIT    ): game_quit(); break;
    }
}
