/*
 * g_utils.c
 *
 *  Created on: 4 авг. 2018 г.
 *      Author: mastersan
 */

#include "g_defs.h"

#include "sys_utils.h"
#include "Z_mem.h"

#include <stdlib.h>

static char * game_dir_home = NULL;
static char * game_dir_conf = NULL;
static char * game_file_scores = NULL;

int game_directories_init(const char * homedir)
{
    game_dir_home  = app_path_build(homedir, GAME_DIR_REL_HOME );
    if(game_dir_home == NULL)
    {
        return -1;
    }
    game_dir_conf  = app_path_build(homedir, GAME_DIR_REL_CONF );
    if(game_dir_conf == NULL)
    {
        return -1;
    }
    game_file_scores = app_path_build(homedir, GAME_FILE_REL_SCORES);
    if(game_file_scores == NULL)
    {
        return -1;
    }

    app_directory_check(game_dir_home);
    app_directory_check(game_dir_conf);
    return 0;
}

void game_directories_done(void)
{
    Z_free(game_dir_home);
    Z_free(game_dir_conf);
    Z_free(game_file_scores);
}

const char * game_dir_home_get(void)
{
    /*  TODO: if(game_dir_home == NULL) GAME_ABORT(); */
    return game_dir_home;
}

const char * game_dir_conf_get(void)
{
    return game_dir_conf;
}

const char * game_file_scores_get(void)
{
    return game_file_scores;
}



