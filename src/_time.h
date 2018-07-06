/**
 * system timer
 */
#ifndef SRC_TIME_H_
#define SRC_TIME_H_

typedef unsigned long game_time_s_t;
typedef unsigned long game_time_ms_t;

game_time_s_t system_getTime_realTime_s(void);
game_time_ms_t system_getTime_realTime_ms(void);

#endif
