/*
 * ents_payer_death.c
 *
 *  Created on: 9 авг. 2018 г.
 *      Author: mastersan
 */

#include "ents_chart.h"
#include "ents_payer_death.h"

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
    ctx->rec.weight = g_ctl_player_weight();
    ctx->rec.scores = g_ctl_player_scores();
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

    if(!ctx->top10)
    {
        return false;
    }

    switch(key)
    {
        case IO_KB_BACKSPACE:
        {
            if(ctx->count > 0)
            {
                rec->name[ctx->count] = '\0';
                --ctx->count;
            }
            break;
        }
        case IO_KB_ENTER:
        {
            chart_insert(rec);
            return true;
        }
        default:
        {
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

    text.c.atr = 0x0F;
    while(i < ctx->count && rec->name[i] != '\0')
    {
        text.c.chr = rec->name[i];
        text_setch(31 + i, 21);
        ++i;
    }
    text.c.chr = 176;
    while(i < MENU_DEATH_NAME_MAXLEN)
    {
        text_setch(31 + i, 21);
        ++i;
    }
}

void menu_death_draw_on_enter(void)
{
    struct menu_death_ctx * ctx = &menu_death_ctx;

    text.c.atr=0x0F;
    text_print(32,  3, "Tы типа сдох :-(");
    text.c.atr=0x2F;
    text_print(30,  5, "  ****************  ");
    text_print(30,  6, " *                * ");
    text_print(30,  7, "*   \\ /      \\ /   *");
    text_print(30,  8, "*    X        X    *");
    text_print(30,  9, "*   / \\  **  / \\   *");
    text_print(30, 10, "*        **        *");
    text_print(30, 11, "*        **        *");
    text_print(30, 12, "*        **        *");
    text_print(30, 13, "*                  *");
    text_print(30, 14, "*    ==========    *");
    text_print(30, 15, "*   /          \\   *");
    text_print(30, 16, "*                  *");
    text_print(30, 17, " **              ** ");
    text_print(30, 18, "   **************   ");
    text_print(26, 20, "СОЖРАЛ КОНОПЛИ(КГ): ");
    text_print(26+20, 20, "%d", (int)g_ctl_player_scores());

    if(!ctx->top10)
    {
        text_print(35, 21, "ТЫ ХУДШИЙ!");
    }
    else
    {
        text_print(26, 21, "ИМЯ> ");
    }
    menu_death_print_name(ctx);
}

void menu_death_draw_on_update(void)
{
    struct menu_death_ctx * ctx = &menu_death_ctx;
    menu_death_print_name(ctx);
}
