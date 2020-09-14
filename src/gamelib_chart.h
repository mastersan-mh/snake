/*
 * gamelib_chart.h
 *
 *  Created on: 25 дек. 2017 г.
 *      Author: mastersan
 */

#ifndef SRC_GAMELIB_CHART_H_
#define SRC_GAMELIB_CHART_H_

#include "g_eng_exports.h"

#include <stddef.h>
#include <stdint.h>

#define ENTS_CHARTREC_NAME_SIZE 16

/**
 * @brief Record of the winners table
 */
struct chartrec
{
    char     name[ENTS_CHARTREC_NAME_SIZE];
    uint32_t weight;
    uint32_t scores;
};

void chart_load(void);
void chart_save(void);
size_t chart_len(void);
const struct chartrec * chart_row_get(size_t row);
bool chart_record_in_chart(const struct chartrec *rec);
void chart_insert(const struct chartrec *rec);

#endif /* SRC_GAMELIB_CHART_H_ */
