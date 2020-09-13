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
#include "ringbuf.h"

#include "g_utils.h"
#include "render.h"
#include "world_main.h"

#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdatomic.h>

/* #define USE_TICK_LATE */

#define SYSEVENTS_MAX  128
#define EVENTS_MAX  SYSEVENTS_MAX

struct g_event
{
    enum g_event_type type;
    struct g_event_data data;
};

struct g_sysevent
{
    enum g_sysevent_type type;
    struct g_sysevent_data data;
};

struct g_events
{
    volatile atomic_bool sig_raised_winch;
    struct g_sysevent sysevents[SYSEVENTS_MAX];
    ringbuf_t sysevents_ring;

    struct g_event events[EVENTS_MAX];
    ringbuf_t events_ring;

};

static struct g_events g_events;

struct timespec ts_tick_next;

static int fd_n;
static fd_set readfds;

static clockid_t clock_id;

#if 1
#   define DEBUG_PRINT(format, ...)
#   define DEBUG_PRINT_XY(x, y, format, ...)
#else
#   define DEBUG_PRINT(format, ...) __func__;
#   define DEBUG_PRINT_XY(x, y, format, ...) debug_print(x, y, format, ##__VA_ARGS__)
#endif


#include <signal.h>

struct sigaction act;

static void P_raise_winch(void);

static void P_sa_sigaction(int signum, siginfo_t * siginfo, void * additional)
{
    P_raise_winch();
}

int g_events_init(void)
{
    atomic_init(&g_events.sig_raised_winch, false);

    ringbuf_init(&g_events.sysevents_ring, SYSEVENTS_MAX);
    ringbuf_init(&g_events.events_ring, EVENTS_MAX);

    int res;

    /* signals */
    act.sa_sigaction = P_sa_sigaction,
    act.sa_flags = SA_SIGINFO /* | SA_NOMASK */;
    sigemptyset (&act.sa_mask);
    res = sigaction(SIGWINCH, &act, NULL);
    if(res)
    {
        return -1;
    }

    /* timers */
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

void g_events_done(void)
{

}

/**
 * @note Call it from signal handler
 */
static void P_raise_winch(void)
{
    atomic_store(&g_events.sig_raised_winch, true);
}

/**
 * @brief Check, witch signal is raised
 */
static void P_check_sig_raised(void)
{
    bool value;

    value = atomic_load(&g_events.sig_raised_winch);
    if(value)
    {
        atomic_store(&g_events.sig_raised_winch, false);
        g_event_send(G_SYSEVENT_VID_WINCH, NULL);
    }

}


void g_events_handle(void)
{
    while(!ringbuf_check_empty(&g_events.sysevents_ring))
    {
        size_t head = ringbuf_head_get(&g_events.sysevents_ring);
        struct g_sysevent * sysevent = &g_events.sysevents[head];

        switch(sysevent->type)
        {
            case G_SYSEVENT_VID_WINCH:
            {
                render_winch();
                break;
            }
            case G_SYSEVENT_KEYBOARD:
            {
                if(ringbuf_check_full(&g_events.events_ring))
                {
                    ERROR("Keyboard events queue overflow");
                }
                size_t tail = ringbuf_tail_get(&g_events.events_ring);
                struct g_event * event = &g_events.events[tail];
                event->type = G_EVENT_KEYBOARD;
                event->data.KEYBOARD.key = sysevent->data.KEYBOARD.key;
                ringbuf_enqueue(&g_events.events_ring);
                break;
            }
            case G_SYSEVENT_TICK:
            {
                render_clearbuf();
                g_ctl_game_tick();
                world_add_to_render();
                ringbuf_flush(&g_events.events_ring);
                break;
            }
            case G_SYSEVENT_STOP_GAME_TICKS:
            {
                /* game_destroy(); */
                break;
            }
        }

        ringbuf_dequeue(&g_events.sysevents_ring);
    }
}

bool g_events_is_empty(void)
{
    return ringbuf_check_empty(&g_events.sysevents_ring);
}

void g_events_flush(void)
{
    ringbuf_flush(&g_events.sysevents_ring);
}

void g_event_send(
    enum g_sysevent_type type,
    const struct g_sysevent_data * data
)
{
    if(ringbuf_check_full(&g_events.sysevents_ring))
    {
        ERROR("System queue overflow");
        return;
    }

    size_t tail = ringbuf_tail_get(&g_events.sysevents_ring);
    struct g_sysevent * event = &g_events.sysevents[tail];
    event->type = type;
    if(data != NULL)
        event->data = *data;
    ringbuf_enqueue(&g_events.sysevents_ring);
}

static game_time_ms_t events_tick_time = 500;

void g_event_ticktime_set(game_time_ms_t ticktime)
{
    events_tick_time = ticktime;
}

void g_events_pump(void)
{

    /* debug */
    static long intr_count = 0;
    static long tick_n = 0;
    static long tick_on_ERROR_n = 0;
    static long tick_on_read_n = 0;
    static long tick_on_timeout_n = 0;
#ifdef USE_TICK_LATE
    static long tick_late_count = 0;
#endif

    struct g_sysevent_data data;
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
        if(errno == EINTR)
        {
            P_check_sig_raised();
            ++intr_count;
        }

        if(ts_timercmp(&ts_now, &ts_tick_next,  >=))
        {
            tick = true;
            ++tick_on_ERROR_n;
            DEBUG_PRINT("TICK ON ERROR\n");
        }
    }
    else if(res == 0)
    {
        /* timeout */
        tick = true;

        DEBUG_PRINT("TICK ON TIMEOUT");
        ++tick_on_timeout_n;

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
            g_event_send(G_SYSEVENT_KEYBOARD, &data);
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
#endif

        ts_timeradd(&ts_now, &ts_events_ticktime, &ts_tick_next);

        data.TICK.time = ts_now;
        g_event_send(G_SYSEVENT_TICK, NULL);

        tick_n++;

    }


    DEBUG_PRINT_XY(1, 1, "tick_n            = %d" , (long)tick_n);
    DEBUG_PRINT_XY(1, 2, "tick_on_timeout_n = %ld", (long)tick_on_timeout_n);
    DEBUG_PRINT_XY(1, 3, "tick_on_read_n    = %ld", (long)tick_on_read_n);

    DEBUG_PRINT_XY(1, 5, "events_tick_time    = %ld" , (long)events_tick_time);
    DEBUG_PRINT_XY(1, 6, "tv_events_tick_time = " G_PRI_ts(), G_FMT_ts(&ts_events_ticktime));
    DEBUG_PRINT_XY(1, 7, "timeout             = " G_PRI_ts(), G_FMT_ts(&timeout));

    DEBUG_PRINT_XY(1, 9, "ts_tick_next        = " G_PRI_ts(), G_FMT_ts(&ts_tick_next));
#ifdef USE_TICK_LATE
    DEBUG_PRINT_XY(1, 11, "TICK: tick_late_count = %ld", (long)tick_late_count);
    DEBUG_PRINT_XY(1, 12, "TICK: ts_tick_late    = " G_PRI_ts(), G_FMT_ts(&ts_tick_late));
#endif

}

bool g_events_event_pump(
        enum g_event_type * type,
        struct g_event_data * data
)
{
    if(ringbuf_check_empty(&g_events.events_ring))
    {
        return false;
    }

    size_t head = ringbuf_head_get(&g_events.events_ring);
    struct g_event * event = &g_events.events[head];
    *type = event->type;
    *data = event->data;
    ringbuf_dequeue(&g_events.events_ring);
    return true;
}
