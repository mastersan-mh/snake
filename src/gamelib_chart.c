/*
 * gamelib_chart.c
 *
 *  Created on: 25 дек. 2017 г.
 *      Author: mastersan
 */

#include "gamelib_chart.h"

#include "gamelib_ctrl.h"

#define CHART_RMAX 10

#include <stdio.h>

/**
 * @brief таблица победителей
 */
struct __attribute__((packed)) chart
{
    struct chartrec r[CHART_RMAX];
    int num;//количество записей
};

static struct chart chart;

/*
 * прочитать файл с таблицей
 */
void chart_load(void)
{
    FILE *F;
    chart.num = 0;


    if((F = fopen(gamelib.filename_scores, "rb")) == NULL)
    {
        return;
    }

    do{
        if(fread(&chart.r[chart.num], sizeof(struct chartrec), 1, F) == 1)
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

    if((F = fopen(gamelib.filename_scores, "wb")) == NULL)
    {
        return;
    }
    fwrite(chart.r, sizeof(struct chartrec), chart.num, F);
    fclose(F);
}

/**
 * @brief insert player's scores into chart
 */
void chart_insert(const struct chartrec *rec)
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
const struct chartrec * chart_row_get(size_t row)
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
bool chart_record_in_chart(const struct chartrec *rec)
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
