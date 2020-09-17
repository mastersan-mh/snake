/*
 * utils.h
 *
 *  Created on: 4 авг. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_SYS_UTILS_H_
#define SRC_SYS_UTILS_H_

#include "g_types.h"

#include <stdlib.h>

#ifndef ARRAY_SIZE
#   define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

char * app_path_build(
        const char * path1,
        size_t path1_maxlen,
        const char * path2,
        size_t path2_maxlen
);

/**
 * @brief Check the directory. If not exist - create it
 */
int app_directory_check(const char * path);

bool str_key_is_character(int key);


#endif /* SRC_SYS_UTILS_H_ */
