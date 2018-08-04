/*
 * utils.c
 *
 *  Created on: 4 авг. 2018 г.
 *      Author: mastersan
 */

#include "g_defs.h"

#include "sys_utils.h"

#include "Z_mem.h"

#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

char * app_path_build(
        const char * path1,
        const char * path2
)
{
    char * path;
    size_t path1_len = strnlen(path1, GAME_FILENAME_LEN);
    size_t path2_len = strnlen(path2, GAME_FILENAME_LEN);
    path = Z_malloc(path1_len + path2_len + 1);
    if(path == NULL)
    {
        return NULL;
    }
    strncpy(path, path1, path1_len);
    strncat(path, path2, path2_len);
    return path;
}

void app_directory_check(const char * path)
{
    DIR * dir = opendir(path);
    if(dir == NULL)
    {
        mkdir(path, 0755);
    }
    else
    {
        closedir(dir);
    }
}

