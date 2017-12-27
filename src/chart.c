/*
 * chart.c
 *
 *  Created on: 25 дек. 2017 г.
 *      Author: mastersan
 */

#include "chart.h"

#define CHART_RMAX 10

#include <stdio.h>
/**
 * @brief таблица победителей
 */
typedef struct
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
    if((F = fopen("snake.frg","rb")) == NULL)
    {
        return;
    }

    do{
        if(fread(&chart.r[chart.num], sizeof(chartrec_t), 1, F) == 1)
        {
            chart.num++;
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

    if((F = fopen("snake.frg","wb")) == NULL)
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
    int count;
    int i;
    count=0;
    while(count < chart.num && rec->scores < chart.r[count].scores)
    {
        count++;
    }
    if(count >= CHART_RMAX)
    {
        return; /* out of chart */
    }
    chart.num++;
    for(i = chart.num - 1; i > count; --i)
    {
        chart.r[i] = chart.r[i - 1];
    }
    chart.r[count] = *rec;
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
 * попали в таблицу рекордов или нет
 * вход:
 * rec - данные игрока
 * выход:
 * 0 - не попали
 * 1 - попали
 */
int chart_in_chart(const chartrec_t *rec)
{
    int count = 0;
    while(count < chart.num && rec->scores < chart.r[count].scores)
    {
        count++;
    }
    if(count >= CHART_RMAX)
    {
        return 0; /* out of chart */
    }
    return 1;
}
