/*
 * g_utils.h
 *
 *  Created on: 4 авг. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_G_UTILS_H_
#define SRC_G_UTILS_H_

#include <stdio.h>

#define ERROR(format, ...) fprintf(stderr, format, ##__VA_ARGS__)
#if 1
#   define DEBUG_PRINT(format, ...)
#   define DEBUG_PRINT_XY(x, y, format, ...)
#else
#   define DEBUG_PRINT(format, ...) __func__;
#   define DEBUG_PRINT_XY(x, y, format, ...) debug_print(x, y, format, ##__VA_ARGS__)
#endif

#endif /* SRC_G_UTILS_H_ */
