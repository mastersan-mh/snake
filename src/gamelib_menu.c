/*
 * gamelib_menu.c
 *
 *  Created on: 2 сент. 2018 г.
 *      Author: mastersan
 */

#include "gamelib_menu.h"

#include "gamelib_common.h"
#include "gamelib_ctrl.h"
#include "gamelib_chart.h"

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

struct menu
{
    void (*event_on_enter)(void * ctx);
    void (*event_on_exit)(void * ctx);
    enum imenu (*event_on_event)(int key, void * ctx);
    void (*draw)(void * ctx);
    void * ctx;
};

static const char sys_progversion[] = "SNAKE ver 1.55 (modif: 03.05.2007 ,create(v0.1b): 25.03.2004)";
static const char sys_special    [] = "Здесь никогда не будет вашей рекламы";

static const char rotator[] = "|/-\\";
#define ROTATOR_FRAMES  (sizeof(rotator) - 1)

int rotator_frame_next(int frame)
{
    if(frame >= ROTATOR_FRAMES - 1)
    {
        return 0;
    }
    return frame + 1;
}

int rotator_frame_prev(int frame)
{
    if(frame <= 0)
    {
        return ROTATOR_FRAMES - 1;
    }
    return frame - 1;
}

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
    int frame_l;
    int frame_r;
    int waiter;
} menu_main_ctx = {};

static enum imenu menu_main_on_event(int key, void * ctx_)
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
    char left = rotator[ctx->frame_l];
    char right = rotator[ctx->frame_r];

    if(ctx->waiter >= 5)
    {
        ctx->waiter = 0;
        ctx->frame_l = rotator_frame_next(ctx->frame_l);
        ctx->frame_r = rotator_frame_prev(ctx->frame_r);
    }
    else
    {
        ctx->waiter++;
    }

    menu_print((80 - 10) / 2 - 3 , 12 + ctx->sub, TEXT_ATR, "%c->", left);
    menu_print((80 - 10) / 2 + 10, 12 + ctx->sub, TEXT_ATR, "<-%c", right);

}

/**
 * @brief New game 0
 */
static void menu_newgame0_on_enter(void * ctx_)
{
    gamelib_game_create(0);
}

/**
 * @brief New game 1
 */
static void menu_newgame1_on_enter(void * ctx_)
{
    gamelib_game_create(1);
}

/**
 * @brief New game 2
 */
static void menu_newgame2_on_enter(void * ctx_)
{
    gamelib_game_create(2);
}

/**
 * @brief Chart table
 */
static enum imenu menu_chart_event_on_event(int key, void * ctx_)
{
    return IMENU_MAIN;
}

static void menu_chart_draw(void * ctx_)
{
    static const char anti_war[] = "Нет войне! Даешь Rock-N-Roll!";

    size_t row;
    int lev;

#undef TEXT_ATR
#define TEXT_ATR (0x09)
    menu_print(20, 7, TEXT_ATR, "МЕСТО ИМЯ             ФРАГИ  ВЕС    СТАТУС");

    size_t len = chart_len();

    for(row = 1; row <= len; ++row)
    {
        const struct chartrec *rec = chart_row_get(row - 1);
        lev = rec->scores/SCORES_PER_LEVEL;
        if(lev > LEVEL_MAX - 1)
        {
            lev = LEVEL_MAX - 1;
        }
        menu_print(20, 7 + row, TEXT_ATR, "%-5d %-15s %-6d %-6d %-20s"
                , (int)row
                , rec->name
                , (int)rec->scores
                , (int)rec->weight
                , (level_str[lev])
        );
    }

#undef TEXT_ATR
#define TEXT_ATR (0x5f)
    menu_print((80 - 29) / 2, 22, TEXT_ATR, anti_war);

#undef TEXT_ATR
#define TEXT_ATR (0x8F)
    menu_print((80-16)/2, 23, TEXT_ATR, "PRESS ANY KEY...");
}

/**
 * @brief Help
 */
static enum imenu menu_help_event_on_event(int key, void * ctx_)
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


static const struct menu menus[] =
{
        { NULL, NULL, menu_main_on_event, menu_main_draw , &menu_main_ctx },/* IMENU_MAIN     */
        { menu_newgame0_on_enter, NULL, NULL, NULL, NULL },/* IMENU_NEWGAME0 */
        { menu_newgame1_on_enter, NULL, NULL, NULL, NULL },/* IMENU_NEWGAME1 */
        { menu_newgame2_on_enter, NULL, NULL, NULL, NULL },/* IMENU_NEWGAME2 */
        { NULL, NULL, menu_chart_event_on_event, menu_chart_draw, NULL },/* IMENU_CHART    */
        { NULL, NULL, menu_help_event_on_event , menu_help_draw, NULL },/* IMENU_HELP     */
        { menu_quit_on_enter    , NULL, NULL, NULL, NULL },/* IMENU_QUIT     */
};

static enum imenu m_imenu_prev = IMENU_MAIN;
static enum imenu m_imenu;

static void P_menu_change(enum imenu imenu)
{
    m_imenu = imenu;
    if(m_imenu_prev != m_imenu)
    {
        /* Menu was changed */
        const struct menu * menu_old = &menus[m_imenu_prev];
        if(menu_old->event_on_exit != NULL)
        {
            menu_old->event_on_exit(menu_old->ctx);
        }

        const struct menu * menu_new = &menus[m_imenu];
        if(menu_new->event_on_enter != NULL)
        {
            menu_new->event_on_enter(menu_new->ctx);
        }
        m_imenu_prev = m_imenu;
    }
}


void menu_handle_input(int key)
{
    const struct menu * menu = &menus[m_imenu];
    void * ctx = menu->ctx;
    enum imenu imenu;
    if(menu->event_on_event != NULL)
    {
        imenu = menu->event_on_event(key, ctx);
    }
    else
    {
        imenu = IMENU_MAIN;
    }

    P_menu_change(imenu);
}

void menu_draw(void)
{

    gamelib.geng->render_background(0x00, ' ');

    const struct menu * menu = &menus[m_imenu];

    if(menu->draw != NULL)
    {
        void * ctx = menu->ctx;
        menu->draw(ctx);
    }
}

void menu_show_menu(enum imenu imenu)
{
    P_menu_change(imenu);
    gamelib.showmenu = true;
}
