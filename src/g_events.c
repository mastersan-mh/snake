/*
 * events.c
 *
 *  Created on: 18 июля 2018 г.
 *      Author: mastersan
 */


#include "g_events.h"

#include "Z_mem.h"
#include "io.h"

#include "game.h"

#include "sys_time.h"
#include "_text.h"

#include <stdint.h>
#include <unistd.h>
#include <sys/select.h>

#define USE_TICK_LATE

static event_head_t events;

struct timespec ts_tick_next;

static int fd_n;
static fd_set readfds;

static clockid_t clock_id;

#if 1
#   define DEBUG_PRINT(format, ...)
#   define DEBUG_PRINT_XY(x, y, format, ...)
#else
#   define DEBUG_PRINT(format, ...) __func__;
#   define DEBUG_PRINT_XY(x, y, format, ...) text_print(x, y, format, ##__VA_ARGS__)
#endif

int g_events_init(void)
{
    CIRCLEQ_INIT(&events);

    int res;
    struct timespec ts_resolution;

    res = app_time_clock_id_get(
            &clock_id,
            &ts_resolution
    );

    if(res)
    {
        return -1;
    }

    DEBUG_PRINT("clock_id = %d\n", (int)clock_id );
    DEBUG_PRINT("ts_resolution = " PRI_ts_x(12, 9) "\n", FMT_ts(ts_resolution) );

    fd_n = 1;
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);

    ts_tick_next = TS_ZERO;

    return 0;
}

void g_events_handle(void)
{
    while(!CIRCLEQ_EMPTY(&events))
    {
        event_t * event = CIRCLEQ_FIRST(&events);
        CIRCLEQ_REMOVE(&events, event, queue);
        g_event_handle(event);
        Z_free(event);
    }
}

bool g_events_is_empty(void)
{
    return CIRCLEQ_EMPTY(&events);
}

void g_events_flush(void)
{
    while(!CIRCLEQ_EMPTY(&events))
    {
        event_t * event = CIRCLEQ_FIRST(&events);
        CIRCLEQ_REMOVE(&events, event, queue);
        Z_free(event);
    }
}

void g_event_send(
    event_type_t type,
    const event_data_t * data
)
{
    event_t * event = Z_malloc(sizeof(event_t));
    event->type = type;
    if(data != NULL)
        event->data = *data;

    CIRCLEQ_INSERT_TAIL(&events, event, queue);
}

static game_time_ms_t events_tick_time = 500;

void g_event_ticktime_set(game_time_ms_t ticktime)
{
    events_tick_time = ticktime;
}

void g_events_pump(void)
{
    /* debug */
    static long tick_n = 0;
    static long tick_on_read_n = 0;
    static long tick_on_timeout_n = 0;
#ifdef USE_TICK_LATE
    static long tick_late_count = 0;
#endif

    event_data_t data;
    /****/
    int res;

#ifdef USE_TICK_LATE
    struct timespec ts_tick_late;
#endif
    struct timespec ts_now;

    struct timespec ts_timeout;
    struct timespec timeout;

    bool tick = false;

    struct timespec ts_events_ticktime;

    app_time_ms_to_timespec(events_tick_time, &ts_events_ticktime);

    fd_set rfds = readfds;
    fd_set * rfdsp = &rfds;

    clock_gettime(clock_id, &ts_now);

    app_ts_timeout_compute(
            &ts_now,
            &ts_tick_next,
            &ts_timeout
    );
    timeout = ts_timeout;

    /*
    DEBUG_PRINT("ts_now = " PRI_ts_x(12, 9) "\n", FMP_ts(ts_now) );
    DEBUG_PRINT("timeout = " PRI_ts_x(12, 9) "\n", FMP_ts(timeout) );
    */

    res = pselect(fd_n, rfdsp, NULL, NULL, &timeout, NULL);

    clock_gettime(clock_id, &ts_now);


    if(res < 0)
    {
        /* error */
    }
    else if(res == 0)
    {
        /* timeout */
        tick = true;

        DEBUG_PRINT("TICK ON TIMEOUT");
        tick_on_timeout_n++;

    }
    else
    {


        if(FD_ISSET(0, rfdsp))
        {
/*
            int key = 0;
            read(0, &key, 1);
            DEBUG_PRINT("key = %d\n", key);
 */

            int key = io_getch();

            data.KEYBOARD.key = key;
            g_event_send(G_EVENT_KEYBOARD, &data);
        }


        if(ts_timercmp(&ts_now, &ts_tick_next,  >=))
        {
            tick = true;
            ++tick_on_read_n;
            DEBUG_PRINT("TICK ON READ\n");
        }


    }

    if(tick)
    {

#ifdef USE_TICK_LATE
        ts_timersub(&ts_now, &ts_tick_next, &ts_tick_late);
        if(ts_timercmp(&ts_tick_late , &TS_ZERO, > ))
        {
            ++tick_late_count;
        }
        DEBUG_PRINT_XY(1, 11, "TICK: tick_late_count = %ld", (long)tick_late_count);
        DEBUG_PRINT_XY(1, 12, "TICK: ts_tick_late    = " G_PRI_ts(), G_FMT_ts(&ts_tick_late));
#endif

        ts_timeradd(&ts_now, &ts_events_ticktime, &ts_tick_next);

        data.TICK.time = ts_now;
        g_event_send(G_EVENT_TICK, NULL);

        tick_n++;

    }


    DEBUG_PRINT_XY(1, 1, "tick_n            = %d" , (long)tick_n);
    DEBUG_PRINT_XY(1, 2, "tick_on_timeout_n = %ld", (long)tick_on_timeout_n);
    DEBUG_PRINT_XY(1, 3, "tick_on_read_n    = %ld", (long)tick_on_read_n);

    DEBUG_PRINT_XY(1, 5, "events_tick_time    = %ld" , (long)events_tick_time);
    DEBUG_PRINT_XY(1, 6, "tv_events_tick_time = " G_PRI_ts(), G_FMT_ts(&ts_events_ticktime));
    DEBUG_PRINT_XY(1, 7, "timeout             = " G_PRI_ts(), G_FMT_ts(&timeout));

    DEBUG_PRINT_XY(1, 9, "ts_tick_next        = " G_PRI_ts(), G_FMT_ts(&ts_tick_next));

}

