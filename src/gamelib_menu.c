/*
 * gamelib_menu.c
 *
 *  Created on: 2 сент. 2018 г.
 *      Author: mastersan
 */

#include "gamelib_ctrl.h"

#include "gamelib_menu.h"

#include "gamelib_common.h"

#include <string.h>

/**
 * @brief Print menu string with it attributes
 * @param[in] x         coord. x
 * @param[in] y         coord. y
 * @param[in] atr       attributes
 * @param[in] format    string
 */
#define menu_print(x, y, atr, format, ...) \
            gamelib.geng->print((x), (y), (atr), (format), ##__VA_ARGS__)

#define SYS_SPECIAL_LEN (sizeof(sys_special) - 1)

typedef struct
{
    void (*event_on_enter)(void * ctx);
    void (*event_on_exit)(void * ctx);
    menu_index_t (*event_on_event)(int key, void * ctx);
    void (*draw)(void * ctx);
    void * ctx;
} menu_t;

static const char sys_progversion[] = "SNAKE ver 1.55 (modif: 03.05.2007 ,create(v0.1b): 25.03.2004)";
static const char sys_special    [] = "Здесь никогда не будет вашей рекламы";


static void P_menu_dec(int menu_amount, int * imenu)
{
    if((*imenu) <= 0)
    {
        *imenu = menu_amount - 1;
        return;
    }
    --(*imenu);
}

static void P_menu_inc(int menu_amount, int * imenu)
{
    if((*imenu) >= menu_amount - 1)
    {
        *imenu = 0;
        return;
    }
    ++(*imenu);
}


/**
 * @brief Main menu
 */
static struct menu_main_ctx
{
    int sub;
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

static void menu_main_draw(void * ctx_)
{
    struct menu_main_ctx * ctx = ctx_;

#undef TEXT_ATR
#define TEXT_ATR (0x09)
    menu_print(29, 1, TEXT_ATR, "**** **  ** ***********");
    menu_print(29, 2, TEXT_ATR, "   * ** *** *     **  *");
    menu_print(29, 3, TEXT_ATR, "**** **** * ***** *****");
    menu_print(29, 4, TEXT_ATR, "***  * ** * ***** ** **");
    menu_print(29, 5, TEXT_ATR, "  ** *    * *      * **");
    menu_print(29, 6, TEXT_ATR, "**** *    * ***** ** **");
    menu_print(29, 7, TEXT_ATR, "******    ******* *  **");

    menu_print(40-(SYS_SPECIAL_LEN / 2), 9, TEXT_ATR, sys_special);

    menu_print(1, 24,  TEXT_ATR, sys_progversion);

#undef TEXT_ATR
#define TEXT_ATR (0x5F)
    menu_print((80-10)/2, 12, TEXT_ATR, " НОВАЯ 1  ");
    menu_print((80-10)/2, 13, TEXT_ATR, " НОВАЯ 2  ");
    menu_print((80-10)/2, 14, TEXT_ATR, " НОВАЯ 3  ");
    menu_print((80-10)/2, 15, TEXT_ATR, "ПОБЕДИТЕЛИ");
    menu_print((80-10)/2, 16, TEXT_ATR, "  ПОМОЩЬ  ");
    menu_print((80-10)/2, 17, TEXT_ATR, "  ВЫХОД   ");

#undef TEXT_ATR
#define TEXT_ATR (0x0F)
    menu_print( 7, 22, TEXT_ATR, "Mad House Software");
    menu_print(10, 23, TEXT_ATR, "Programming: Ремнёв Александр a.k.a. MasterSan[MH]");

#undef TEXT_ATR
#define TEXT_ATR (0x05)
    menu_print((80 - 10) / 2 - 2 , 12 + ctx->sub, TEXT_ATR, "->");
    menu_print((80 - 10) / 2 + 10, 12 + ctx->sub, TEXT_ATR, "<-");

}

/**
 * @brief New game 0
 */
static void menu_newgame0_on_enter(void * ctx_)
{
    gamelib.stage = 0;
    gamelib_game_create();
}

/**
 * @brief New game 1
 */
static void menu_newgame1_on_enter(void * ctx_)
{
    gamelib.stage = 1;
    gamelib_game_create();
}

/**
 * @brief New game 2
 */
static void menu_newgame2_on_enter(void * ctx_)
{
    gamelib.stage = 2;
    gamelib_game_create();
}

/**
 * @brief Chart table
 */
static menu_index_t menu_chart_event_on_event(int key, void * ctx_)
{
    return IMENU_MAIN;
}

static void menu_chart_draw(void * ctx_)
{
    gamelib_show_records();

#undef TEXT_ATR
#define TEXT_ATR (0x8F)
    menu_print((80-16)/2, 23, TEXT_ATR, "PRESS ANY KEY...");
    menu_print((80-16)/2, 24, 0x0F, "PRESS ANY KEY1...");
}

/**
 * @brief Help
 */
static menu_index_t menu_help_event_on_event(int key, void * ctx_)
{
    return IMENU_MAIN;
}

static void menu_help_draw(void * ctx_)
{

#undef TEXT_ATR
#define TEXT_ATR (0x09)
    menu_print((80-13)/2 , 5, TEXT_ATR, "ТИПА ЭТО ХЕЛП");
    menu_print(20, 7+ 0, TEXT_ATR, "Управление:");
    menu_print(20, 7+ 1, TEXT_ATR, "Стрелки - Указание направления движения");
    menu_print(20, 7+ 2, TEXT_ATR, "+,-     - Управление скоростью игры");
    menu_print(20, 7+ 3, TEXT_ATR, "P       - Пауза");
    menu_print(20, 7+ 4, TEXT_ATR, "ESC     - Выход в меню");
    menu_print(20, 7+ 6, TEXT_ATR, "Предметы:");
    menu_print(20, 7+ 7, TEXT_ATR, "\5 - конопля");
    menu_print(20, 7+ 8, TEXT_ATR, "\6 - выросшая конопля");
    menu_print(20, 7+ 9, TEXT_ATR, "\13 - пурген");
    menu_print(20, 7+10, TEXT_ATR, "@ - дерьмо");

#undef TEXT_ATR
#define TEXT_ATR (0x8F)
    menu_print((80-16)/2, 23,  TEXT_ATR, "PRESS ANY KEY...");

}

static void menu_quit_on_enter(void * ctx_)
{
    gamelib.geng->game_quit();
}


static const menu_t menus[] =
{
        { NULL, NULL, menu_main_on_event, menu_main_draw , &menu_main_ctx },/* IMENU_MAIN     */
        { menu_newgame0_on_enter, NULL, NULL, NULL, NULL },/* IMENU_NEWGAME0 */
        { menu_newgame1_on_enter, NULL, NULL, NULL, NULL },/* IMENU_NEWGAME1 */
        { menu_newgame2_on_enter, NULL, NULL, NULL, NULL },/* IMENU_NEWGAME2 */
        { NULL, NULL, menu_chart_event_on_event, menu_chart_draw, NULL },/* IMENU_CHART    */
        { NULL, NULL, menu_help_event_on_event , menu_help_draw, NULL },/* IMENU_HELP     */
        { menu_quit_on_enter    , NULL, NULL, NULL, NULL },/* IMENU_QUIT     */
};

static menu_index_t m_imenu_prev = IMENU_MAIN;
static menu_index_t m_imenu;


void menu_handle_input(int key)
{
    const menu_t * menu = &menus[m_imenu];
    void * ctx = menu->ctx;
    if(menu->event_on_event != NULL)
    {
        m_imenu = menu->event_on_event(key, ctx);
    }
    else
    {
        m_imenu = IMENU_MAIN;
    }

    /* Menu was changed */
    if(m_imenu_prev != m_imenu)
    {
        const menu_t * menu_old = &menus[m_imenu_prev];
        if(menu_old->event_on_exit != NULL)
        {
            menu_old->event_on_exit(menu_old->ctx);
        }

        const menu_t * menu_new = &menus[m_imenu];
        if(menu_new->event_on_enter != NULL)
        {
            menu_new->event_on_enter(menu_new->ctx);
        }
        m_imenu_prev = m_imenu;
    }
}

void menu_handle(void)
{

    gamelib.geng->render_background(0x00, ' ');

    const menu_t * menu = &menus[m_imenu];

    if(menu->draw)
    {
        void * ctx = menu->ctx;
        menu->draw(ctx);
    }
}

void menu_show_menu(menu_index_t imenu)
{
    m_imenu = imenu;
    gamelib.showmenu = true;
}
