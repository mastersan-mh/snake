/*
 * menu.c
 */

#include "sys_utils.h"
#include "io.h"
#include "io_keys.h"
#include "g_types.h"
#include "menu.h"
#include "game.h"

#include "_text.h"

#include <string.h>

#define SYS_SPECIAL_LEN (sizeof(sys_special) - 1)

typedef struct
{
    void (*event_on_enter)(void * ctx);
    void (*event_on_exit)(void * ctx);
    menu_index_t (*event_on_event)(int key, void * ctx);
    void (*draw_on_enter)(void * ctx);
    void (*draw_on_exit)(void * ctx);
    void (*draw_on_update)(void * ctx);
    void * ctx;
} menu_t;

char sys_progversion[] = "SNAKE ver 1.55 (modif: 03.05.2007 ,create(v0.1b): 25.03.2004)";
char sys_special    [] = "Здесь никогда не будет вашей рекламы";


static void P_menu_dec(int menu_amount, int * imenu)
{
    if((*imenu) <= 0)
    {
        *imenu = menu_amount - 1;
        return;
    }
    (*imenu)--;
}

static void P_menu_inc(int menu_amount, int * imenu)
{
    if((*imenu) >= menu_amount - 1)
    {
        *imenu = 0;
        return;
    }
    (*imenu)++;
}


/**
 * @brief Main menu
 */
static struct menu_main_ctx
{
    int sub;
    int sub_prev;
} menu_main_ctx = {};

static menu_index_t menu_main_on_event(int key, void * ctx_)
{
    struct menu_main_ctx * ctx = ctx_;

    switch(key)
    {
        case IO_KB_UP:
        {
            P_menu_dec(6, &ctx->sub);
            break;
        }
        case IO_KB_DN:
        {
            P_menu_inc(6, &ctx->sub);
            break;
        }
        case IO_KB_ENTER:
        {
            switch(ctx->sub)
            {
                case 0: return IMENU_NEWGAME0;
                case 1: return IMENU_NEWGAME1;
                case 2: return IMENU_NEWGAME2;
                case 3: return IMENU_CHART;
                case 4: return IMENU_HELP;
                case 5: return IMENU_QUIT;
            }
            break;
        }
    }

    return IMENU_MAIN;
}

static void menu_main_draw_on_enter(void * ctx_)
{
    struct menu_main_ctx * ctx = ctx_;
    text.c.atr=0x00;
    text.c.chr=0x00;
    text_fill_screen();

    text.c.atr=0x09;
    text_print(29,1,"**** **  ** ***********");
    text_print(29,2,"   * ** *** *     **  *");
    text_print(29,3,"**** **** * ***** *****");
    text_print(29,4,"***  * ** * ***** ** **");
    text_print(29,5,"  ** *    * *      * **");
    text_print(29,6,"**** *    * ***** ** **");
    text_print(29,7,"******    ******* *  **");

    text_print(40-(SYS_SPECIAL_LEN / 2), 9, sys_special);

    text_print(1,24,sys_progversion);
    text.c.atr=0x5F;
    text_print((80-10)/2,12," НОВАЯ 1  ");
    text_print((80-10)/2,13," НОВАЯ 2  ");
    text_print((80-10)/2,14," НОВАЯ 3  ");
    text_print((80-10)/2,15,"ПОБЕДИТЕЛИ");
    text_print((80-10)/2,16,"  ПОМОЩЬ  ");
    text_print((80-10)/2,17,"  ВЫХОД   ");
    text.c.atr=0x0F;
    text_print( 7,22,"Mad House Software");
    text_print(10,23,"Programming: Ремнёв Александр a.k.a. MasterSan[MH]");
    text_print((80 - 10) / 2 - 2 , 12 + ctx->sub, "->");
    text_print((80 - 10) / 2 + 10, 12 + ctx->sub, "<-");

}

static void menu_main_draw_on_update(void * ctx_)
{
    struct menu_main_ctx * ctx = ctx_;

    text.c.atr=0x05;
    text_print((80 - 10) / 2 - 2 , 12 + ctx->sub_prev, "  ");
    text_print((80 - 10) / 2 + 10, 12 + ctx->sub_prev, "  ");
    text_print((80 - 10) / 2 - 2 , 12 + ctx->sub, "->");
    text_print((80 - 10) / 2 + 10, 12 + ctx->sub, "<-");
    ctx->sub_prev = ctx->sub;

}

/**
 * @brief New game 0
 */
static void menu_newgame0_on_enter(void * ctx_)
{
    game_start(0);
}

/**
 * @brief New game 1
 */
static void menu_newgame1_on_enter(void * ctx_)
{
    game_start(1);
}

/**
 * @brief New game 2
 */
static void menu_newgame2_on_enter(void * ctx_)
{
    game_start(2);
}

/**
 * @brief Chart table
 */
static menu_index_t menu_chart_event_on_event(int key, void * ctx_)
{
    return IMENU_MAIN;
}

static void menu_chart_draw_on_enter(void * ctx_)
{
    text.c.atr=0x00;
    text.c.chr=0x00;
    text_fill_screen();
    g_ctl_show_records();
    text.c.atr=0x8F;
    text_print((80-16)/2, 23, "PRESS ANY KEY...");
}

/**
 * @brief Help
 */
static menu_index_t menu_help_event_on_event(int key, void * ctx_)
{
    return IMENU_MAIN;
}

static void menu_help_draw_on_enter(void * ctx_)
{
    text.c.atr=0x00;
    text.c.chr=0x00;
    text_fill_screen();
    text.c.atr=0x09;

    text_print((80-13)/2,5   ,"ТИПА ЭТО ХЕЛП");
    text_print(20,7+ 0,"Управление:");
    text_print(20,7+ 1,"Стрелки - Указание направления движения");
    text_print(20,7+ 2,"+,-     - Управление скоростью игры");
    text_print(20,7+ 3,"P       - Пауза");
    text_print(20,7+ 4,"ESC     - Выход в меню");
    text_print(20,7+ 6,"Предметы:");
    text_print(20,7+ 7,"\5 - конопля");
    text_print(20,7+ 8,"\6 - выросшая конопля");
    text_print(20,7+ 9,"\13 - пурген");
    text_print(20,7+10,"@ - дерьмо");

    text.c.atr=0x8F;
    text_print((80-16)/2,23,"PRESS ANY KEY...");

}

static void menu_quit_on_enter(void * ctx_)
{
    game_quit();
}


static const menu_t menus[] =
{
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL },/* IMENU_NONE     */
        { NULL, NULL, menu_main_on_event, menu_main_draw_on_enter , NULL, menu_main_draw_on_update, &menu_main_ctx },/* IMENU_MAIN     */
        { menu_newgame0_on_enter, NULL, NULL, NULL, NULL, NULL, NULL },/* IMENU_NEWGAME0 */
        { menu_newgame1_on_enter, NULL, NULL, NULL, NULL, NULL, NULL },/* IMENU_NEWGAME1 */
        { menu_newgame2_on_enter, NULL, NULL, NULL, NULL, NULL, NULL },/* IMENU_NEWGAME2 */
        { NULL, NULL, menu_chart_event_on_event, menu_chart_draw_on_enter, NULL, NULL, NULL },/* IMENU_CHART    */
        { NULL, NULL, menu_help_event_on_event , menu_help_draw_on_enter, NULL, NULL, NULL },/* IMENU_HELP     */
        { menu_quit_on_enter    , NULL, NULL, NULL, NULL, NULL, NULL },/* IMENU_QUIT     */
};

static menu_index_t m_imenu_prev = IMENU_NONE;
static menu_index_t m_imenu = IMENU_MAIN;

static void menu_handle_event_tick(const event_t * event)
{
    /* empty */
}


void menu_handle(const event_t * event)
{

    const menu_t * menu = &menus[m_imenu];
    void * ctx = menu->ctx;
    if(m_imenu_prev != m_imenu)
    {
        if(menu->event_on_enter)
        {
            menu->event_on_enter(ctx);
        }
        if(menu->draw_on_enter)
        {
            menu->draw_on_enter(ctx);
        }
        m_imenu_prev = m_imenu;
    }


    switch(event->type)
    {
        case G_EVENT_TICK:
        {
            menu_handle_event_tick(event);
            break;
        }
        case G_EVENT_KEYBOARD:
        {

            if(menu->event_on_event)
            {
                m_imenu = menu->event_on_event(event->data.KEYBOARD.key, ctx);
            }
            else
            {
                m_imenu = IMENU_MAIN;
            }

            if(menu->draw_on_update)
            {
                menu->draw_on_update(ctx);
            }

            break;
        }
        case G_EVENT_STOP_GAME_TICKS:
        {
            break;
        }
    }


    if(m_imenu_prev != m_imenu)
    {
        if(menu->event_on_exit)
        {
            menu->event_on_exit(ctx);
        }
        if(menu->draw_on_exit)
        {
            menu->draw_on_exit(ctx);
        }
    }


}

void menu_show_menu(menu_index_t imenu)
{
    m_imenu = imenu;
}


