/*
 * chart.h
 *
 *  Created on: 25 дек. 2017 г.
 *      Author: mastersan
 */

#ifndef SRC_CHART_H_
#define SRC_CHART_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief запись таблицы победителей
 */
typedef struct
{
    char     name[16];
    uint32_t weight;
    uint32_t scores;
} chartrec_t;

void chart_load(void);
void chart_save(void);
size_t chart_len(void);
const chartrec_t * chart_row_get(size_t row);
int chart_top(chartrec_t *rec);
void chart_newrec(chartrec_t *rec);

#endif /* SRC_CHART_H_ */
