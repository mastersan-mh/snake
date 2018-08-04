/*
 * g_utils.h
 *
 *  Created on: 4 авг. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_G_UTILS_H_
#define SRC_G_UTILS_H_


#define ERROR(format, ...)

int game_directories_init(const char * homedir);
void game_directories_done(void);

const char * game_dir_home_get(void);
const char * game_dir_conf_get(void);
const char * game_file_scores_get(void);

#endif /* SRC_G_UTILS_H_ */
