/*
 * chart.h
 *
 *  Created on: 25 дек. 2017 г.
 *      Author: mastersan
 */

#ifndef SRC_CHART_H_
#define SRC_CHART_H_

#include "obj.h"

#include <stddef.h>
#include <stdint.h>

#define APP_CHARTREC_NAME_SIZE 16

/**
 * @brief запись таблицы победителей
 */
typedef struct
{
    char     name[APP_CHARTREC_NAME_SIZE];
    uint32_t weight;
    uint32_t scores;
} chartrec_t;

void chart_load(void);
void chart_save(void);
size_t chart_len(void);
const chartrec_t * chart_row_get(size_t row);
bool chart_record_in_chart(const chartrec_t *rec);
void chart_insert(const chartrec_t *rec);

#endif /* SRC_CHART_H_ */
