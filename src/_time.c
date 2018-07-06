/**
 * system timer
 */
#include "_time.h"

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define TICRATE 1

static game_time_s_t lasttimereply_s = 0;
static game_time_s_t basetime_s = 0;

static game_time_ms_t lasttimereply_ms = 0;
static game_time_ms_t basetime_ms = 0;

/**
 * @brief time, seconds
 */
game_time_s_t system_getTime_realTime_s(void)
{
    struct timeval tv;
    game_time_s_t thistimereply;

    gettimeofday(&tv, NULL);

    thistimereply = (tv.tv_sec * TICRATE + (tv.tv_usec * TICRATE) / 1000000);

    /* Fix for time problem */
    if (basetime_s == 0)
    {
        basetime_s = thistimereply;
        thistimereply = 0;
    }
    else
    {
        thistimereply -= basetime_s;
    }
    if (thistimereply < lasttimereply_s)
        thistimereply = lasttimereply_s;

    return (lasttimereply_s = thistimereply);
}

/**
 * @brief time, ms
 */
game_time_ms_t system_getTime_realTime_ms(void)
{
    struct timeval tv;
    game_time_ms_t thistimereply;

    gettimeofday(&tv, NULL);

    thistimereply = (tv.tv_sec * TICRATE * 1000 + (tv.tv_usec * TICRATE) / 1000);

    /* Fix for time problem */
    if (!basetime_ms)
    {
        basetime_ms = thistimereply;
        thistimereply = 0;
    }
    else
    {
        thistimereply -= basetime_ms;
    }
    if (thistimereply < lasttimereply_ms)
        thistimereply = lasttimereply_ms;

    return (lasttimereply_ms = thistimereply);
}

