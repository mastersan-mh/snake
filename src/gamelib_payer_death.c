/*
 * gamelib_payer_death.c
 *
 *  Created on: 9 авг. 2018 г.
 *      Author: mastersan
 */

#include "gamelib_chart.h"
#include "gamelib_objects.h"
#include "gamelib_payer_death.h"
#include "gamelib_ctrl.h"

#include "sys_utils.h"
#include <string.h>

/**
 * @brief The "Death" menu
 */
#define MENU_DEATH_NAME_MAXLEN (ENTS_CHARTREC_NAME_SIZE - 1)
static struct menu_death_ctx
{
    chartrec_t rec;
    bool top10;
    size_t count;
} menu_death_ctx = {};

void menu_death_on_enter(void)
{
    struct menu_death_ctx * ctx = &menu_death_ctx;
    memset(&ctx->rec, 0, sizeof(ctx->rec));
    ctx->rec.weight = player_weight();
    ctx->rec.scores = player_scores();
    ctx->rec.name[0] = '\0';
    ctx->top10 = chart_record_in_chart(&ctx->rec);
    ctx->count = 0;
}

/**
 * @return false - input in process
 * @return true - end of input
 */
bool menu_death_on_event(int key)
{
    struct menu_death_ctx * ctx = &menu_death_ctx;

    chartrec_t * rec = &ctx->rec;

    switch(key)
    {
        case IO_KB_BACKSPACE:
        {
            if(!ctx->top10) break;
            if(ctx->count > 0)
            {
                rec->name[ctx->count] = '\0';
                --ctx->count;
            }
            break;
        }
        case IO_KB_ENTER:
        {
            if(ctx->top10)
            {
                chart_insert(rec);
            }
            return true;
        }
        case IO_KB_ESC:
        {
            if(ctx->top10)
            {
                break;
            }
            return true;
        }
        default:
        {
            if(!ctx->top10) break;
            if(ctx->count < MENU_DEATH_NAME_MAXLEN && str_key_is_character(key))
            {
                rec->name[ctx->count] = key;
                ++ctx->count;
                rec->name[ctx->count] = '\0';
            }
            break;
        }
    }

    return false;
}

static void menu_death_print_name(const struct menu_death_ctx * ctx)
{
    size_t i = 0;
    const chartrec_t * rec = &ctx->rec;

#undef TEXT_ATR
#define TEXT_ATR (0x0f)
    while(i < ctx->count && rec->name[i] != '\0')
    {
        gamelib.geng->putch(31 + i, 21, TEXT_ATR, rec->name[i]);
        ++i;
    }
    while(i < MENU_DEATH_NAME_MAXLEN)
    {
        gamelib.geng->putch(31 + i, 21, TEXT_ATR, 176);
        ++i;
    }
}

void gamelib_intermision_draw(void)
{
    struct menu_death_ctx * ctx = &menu_death_ctx;

#undef TEXT_ATR
#define TEXT_ATR (0x0f)

    gamelib.geng->print(32,  3, TEXT_ATR, "Tы типа сдох :-(");
#undef TEXT_ATR
#define TEXT_ATR (0x2f)
    gamelib.geng->print(30,  5, TEXT_ATR, "  ****************  ");
    gamelib.geng->print(30,  6, TEXT_ATR, " *                * ");
    gamelib.geng->print(30,  7, TEXT_ATR, "*   \\ /      \\ /   *");
    gamelib.geng->print(30,  8, TEXT_ATR, "*    X        X    *");
    gamelib.geng->print(30,  9, TEXT_ATR, "*   / \\  **  / \\   *");
    gamelib.geng->print(30, 10, TEXT_ATR, "*        **        *");
    gamelib.geng->print(30, 11, TEXT_ATR, "*        **        *");
    gamelib.geng->print(30, 12, TEXT_ATR, "*        **        *");
    gamelib.geng->print(30, 13, TEXT_ATR, "*                  *");
    gamelib.geng->print(30, 14, TEXT_ATR, "*    ==========    *");
    gamelib.geng->print(30, 15, TEXT_ATR, "*   /          \\   *");
    gamelib.geng->print(30, 16, TEXT_ATR, "*                  *");
    gamelib.geng->print(30, 17, TEXT_ATR, " **              ** ");
    gamelib.geng->print(30, 18, TEXT_ATR, "   **************   ");
    gamelib.geng->print(26, 20, TEXT_ATR, "СОЖРАЛ КОНОПЛИ(КГ): ");
    gamelib.geng->print(26+20, 20, TEXT_ATR, "%d", (int)player_scores());

    if(!ctx->top10)
    {
        gamelib.geng->print(35, 21, TEXT_ATR, "ТЫ ХУДШИЙ!");
    }
    else
    {
        gamelib.geng->print(26, 21, TEXT_ATR, "ИМЯ> ");
        menu_death_print_name(ctx);
    }
}
