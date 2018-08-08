/*
 * utils.h
 *
 *  Created on: 4 авг. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_SYS_UTILS_H_
#define SRC_SYS_UTILS_H_

#include "g_types.h"

#ifndef ARRAY_SIZE
#   define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

char * app_path_build(
        const char * path1,
        const char * path2
);

void app_directory_check(const char * path);

bool str_key_is_character(int key);


#endif /* SRC_SYS_UTILS_H_ */
