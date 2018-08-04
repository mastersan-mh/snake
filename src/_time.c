/**
 * system timer
 */
#include "_time.h"

#ifndef ARRAY_SIZE
#   define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

const struct timespec ts_zero  = {};

static clockid_t clock_priotity[] =
{
        /* Monotonic system-wide clock.  */
        CLOCK_MONOTONIC,
        /* Monotonic system-wide clock, not adjusted for frequency scaling.  */
        CLOCK_MONOTONIC_RAW,
        /* Identifier for system-wide realtime clock.  */
        CLOCK_REALTIME,
        /* High-resolution timer from the CPU.  */
        CLOCK_PROCESS_CPUTIME_ID,
        /* Thread-specific CPU-time clock.  */
        CLOCK_THREAD_CPUTIME_ID,
        /* Identifier for system-wide realtime clock, updated only on ticks.  */
        CLOCK_REALTIME_COARSE,
        /* Monotonic system-wide clock, updated only on ticks.  */
        CLOCK_MONOTONIC_COARSE,
        /* Monotonic system-wide clock that includes time spent in suspension.  */
        CLOCK_BOOTTIME,
        /* Like CLOCK_REALTIME but also wakes suspended system.  */
        CLOCK_REALTIME_ALARM,
        /* Like CLOCK_BOOTTIME but also wakes suspended system.  */
        CLOCK_BOOTTIME_ALARM,
        /* Like CLOCK_REALTIME but in International Atomic Time.  */
        CLOCK_TAI,
};


int app_time_clock_id_get(
        clockid_t * clock_id,
        struct timespec *ts_resolution
)
{
    size_t i;
    for(i = 0; i < ARRAY_SIZE(clock_priotity); i++)
    {
        *clock_id = clock_priotity[i];
        if(clock_getres(*clock_id, ts_resolution) == 0)
        {
            return 0;
        }
    }
    return -1;
}

void app_ts_timeout_compute(
        const struct timespec * tv_now,
        const struct timespec * tv_next,
        struct timespec * tv_timeout
)
{
    ts_timersub(tv_next, tv_now, tv_timeout);
    if(ts_timercmp(tv_timeout, &TS_ZERO, <))
    {
        *tv_timeout = TS_ZERO;
    }
}

void time_ms_to_timespec(game_time_ms_t time, struct timespec * timespec)
{
    timespec->tv_sec = time/1000;
    timespec->tv_nsec = (time % 1000) * NANOSEC_IN_MILLISEC;
}

