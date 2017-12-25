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
    FILE *F;
    chartrec_t r[CHART_RMAX];
    short num;//количество записей
} chart_t;

static chart_t chart;

//////////////////////////////////////////////////
//прочитать файл с таблицей
//////////////////////////////////////////////////
void chart_load(void)
{
    chart.num=0;
    if((chart.F=fopen("snake.frg","rb"))!=NULL){ //файл присутствует
        do{
            if(fread(&chart.r[chart.num],sizeof(chartrec_t),1,chart.F)==1) chart.num++;
            else break;
        }while(chart.num<CHART_RMAX);
        fclose(chart.F);
    }
}

//////////////////////////////////////////////////
//запмсать таблицу в файл
//////////////////////////////////////////////////
void chart_save(void)
{
    if(chart.num){
        if((chart.F=fopen("snake.frg","wb"))!=NULL){
            fwrite(chart.r,sizeof(chartrec_t)*chart.num,1,chart.F);
            fclose(chart.F);
        }
    }
}

//////////////////////////////////////////////////
//добавить игрока в таблицу
//////////////////////////////////////////////////
void chart_newrec(chartrec_t *rec)
{
    short count,c;
    count=0;
    while(count<chart.num && rec->scores<chart.r[count].scores)count++;
    if(count>=CHART_RMAX)return;//не вошли в таблицу
    if(chart.num<CHART_RMAX)chart.num++;
    for(c=chart.num-1;c>count;c--) chart.r[c]=chart.r[c-1];
    chart.r[count]=*rec;
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
        return NULL;
    return &chart.r[row];
}


//////////////////////////////////////////////////
//попали в таблицу рекордов или нет
//вход:
//rec - данные игрока
//выход:
//0 - не попали
//1 - попали
//////////////////////////////////////////////////
int chart_top(chartrec_t *rec)
{
    int count=0;
    while(count<chart.num && rec->scores<chart.r[count].scores)count++;
    if(count>=CHART_RMAX)return(0);//не вошли в таблицу
    return(1);
}
