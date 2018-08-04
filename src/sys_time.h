/**
 * system timer
 */
#ifndef SRC_SYS_TIME_H_
#define SRC_SYS_TIME_H_

#include <time.h>

#define G_PRI_SP_ts(i, f) "{%" #i "ld[s];%" #f "ld[ns]}"

#define G_PRI_ts() G_PRI_SP_ts(6, 9)

#define G_FMT_ts(ts) (long)(ts)->tv_sec, (long)(ts)->tv_nsec


#define NANOSEC_IN_SEC (1000ll * 1000ll * 1000ll)
#define NANOSEC_IN_MILLISEC (1000ll * 1000ll)

extern const struct timespec ts_zero;

#define TS_ZERO ts_zero


#define ts_timerisset(tvp)    ((tvp)->tv_sec || (tvp)->tv_nsec)

#define ts_timerclear(tvp)    ((tvp)->tv_sec = (tvp)->tv_nsec = 0)

/**
 * @param a         struct timespec
 * @param b         struct timespec
 * @param CPM       > | < | >= | <= | ==
 * @return 0, 1
 */
#define ts_timercmp(a, b, CMP) \
        (((a)->tv_sec == (b)->tv_sec) ? \
                ((a)->tv_nsec CMP (b)->tv_nsec) : \
                ((a)->tv_sec CMP (b)->tv_sec))

#define ts_timeradd(a, b, result) \
        do { \
            (result)->tv_sec = (a)->tv_sec + (b)->tv_sec; \
            (result)->tv_nsec = (a)->tv_nsec + (b)->tv_nsec; \
            if ((result)->tv_nsec >= NANOSEC_IN_SEC) \
            { \
                ++(result)->tv_sec; \
                (result)->tv_nsec -= NANOSEC_IN_SEC; \
            } \
        } while (0)

#define ts_timersub(a, b, result) \
        do { \
            (result)->tv_sec = (a)->tv_sec - (b)->tv_sec; \
            (result)->tv_nsec = (a)->tv_nsec - (b)->tv_nsec; \
            if ((result)->tv_nsec < 0) \
            { \
                --(result)->tv_sec; \
                (result)->tv_nsec += NANOSEC_IN_SEC; \
            } \
        } while (0)

int app_time_clock_id_get(
        clockid_t * clock_id,
        struct timespec *ts_resolution
);

void app_ts_timeout_compute(
        const struct timespec * tv_now,
        const struct timespec * tv_next,
        struct timespec * tv_timeout
);


typedef unsigned long game_time_s_t;
typedef unsigned long game_time_ms_t;

void app_time_ms_to_timespec(game_time_ms_t time, struct timespec * timeval);

#endif /* SRC_SYS_TIME_H_ */
