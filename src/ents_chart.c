/*
 * chart.c
 *
 *  Created on: 25 дек. 2017 г.
 *      Author: mastersan
 */

#include "g_utils.h"

#include "ents_chart.h"

#define CHART_RMAX 10

#include <stdio.h>

/**
 * @brief таблица победителей
 */
typedef struct __attribute__((packed))
{
    chartrec_t r[CHART_RMAX];
    int num;//количество записей
} chart_t;

static chart_t chart;

/*
 * прочитать файл с таблицей
 */
void chart_load(void)
{
    FILE *F;
    chart.num = 0;


    if((F = fopen(game_file_scores_get(), "rb")) == NULL)
    {
        return;
    }

    do{
        if(fread(&chart.r[chart.num], sizeof(chartrec_t), 1, F) == 1)
        {
            ++chart.num;
        }
        else
        {
            break;
        }
    }while(chart.num < CHART_RMAX);
    fclose(F);
}

/**
 * @brief Write table to file
 */
void chart_save(void)
{
    FILE *F;
    if(chart.num == 0)
    {
        return;
    }

    if((F = fopen(game_file_scores_get(), "wb")) == NULL)
    {
        return;
    }
    fwrite(chart.r, sizeof(chartrec_t), chart.num, F);
    fclose(F);
}

/**
 * @brief insert player's scores into chart
 */
void chart_insert(const chartrec_t *rec)
{
    size_t iplace;
    size_t i;
    iplace = 0;
    while(iplace < chart.num && rec->scores < chart.r[iplace].scores)
    {
        ++iplace;
    }
    if(iplace > CHART_RMAX)
    {
        return; /* out of chart */
    }
    if(chart.num < CHART_RMAX)
    {
        ++chart.num;
    }
    for(i = chart.num - 1; i > iplace; --i)
    {
        chart.r[i] = chart.r[i - 1];
    }
    chart.r[iplace] = *rec;
}

size_t chart_len(void)
{
    return chart.num;
}

/**
 * @param row = [0; n)
 */
const chartrec_t * chart_row_get(size_t row)
{
    if(row >= chart.num)
    {
        return NULL;
    }
    return &chart.r[row];
}

/**
 * @brief Will we get into the results table?
 * @param rec       record
 * @return true | false
 */
bool chart_record_in_chart(const chartrec_t *rec)
{
    size_t i = 0;
    while(i < chart.num && rec->scores < chart.r[i].scores)
    {
        i++;
    }
    if(i >= CHART_RMAX)
    {
        return false; /* out of chart */
    }
    return true;
}
