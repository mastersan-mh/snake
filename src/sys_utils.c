/*
 * utils.c
 *
 *  Created on: 4 авг. 2018 г.
 *      Author: mastersan
 */

#include "sys_utils.h"

#include "Z_mem.h"

#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

char * app_path_build(
        const char * path1,
        size_t path1_maxlen,
        const char * path2,
        size_t path2_maxlen
)
{
    char * path;
    size_t path1_len = strnlen(path1, path1_maxlen);
    size_t path2_len = strnlen(path2, path2_maxlen);
    path = Z_malloc(path1_len + path2_len + 1);
    if(path == NULL)
    {
        return NULL;
    }
    strncpy(path, path1, path1_len);
    strncat(path, path2, path2_len);
    return path;
}

int app_directory_check(const char * path)
{
    int res;
    DIR * dir = opendir(path);
    if(dir != NULL)
    {
        closedir(dir);
        res = 0;
    }
    else
    {
        res = mkdir(path, 0755);
    }
    return res;
}

/**
 * @brief Check the key is valid letter
 */
bool str_key_is_character(int key)
{
#if 0
    static const char valid_chars[] =
            "!#$%&'()+,-.0123456789;=@"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`"
            "abcdefghijklmnopqrstuvwxyz{}~"
            "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
            "абвгдежзийклмнопрстуфхцчшщъыьэюяЁё";
    return (strchr(valid_chars, key) != NULL);
#else
    return true;
#endif
}
