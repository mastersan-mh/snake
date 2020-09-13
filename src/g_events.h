/*
 * events.h
 *
 *  Created on: 18 июля 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_G_EVENTS_H_
#define SRC_G_EVENTS_H_

#include "sys_time.h"
#include "g_types.h"

/**
 * @brief Game event type
 */

enum g_event_type
{
    G_EVENT_KEYBOARD
};

/**
 * @brief Game event data
 */
struct g_event_data
{
    union
    {
        struct
        {
            int key;
        } KEYBOARD;
    };
};

/**
 * @brief System event type
 */
enum g_sysevent_type
{
    /* changing window size */
    G_SYSEVENT_VID_WINCH,
    /* keyboard */
    G_SYSEVENT_KEYBOARD,
    /* game tick */
    G_SYSEVENT_TICK,
    /* stop game ticks */
    G_SYSEVENT_STOP_GAME_TICKS
};

/**
 * @brief System event data
 */
struct g_sysevent_data
{
    union
    {
        struct
        {
            /* empty */
        } WINCH;

        struct
        {
            int key;
        } KEYBOARD;

        struct
        {
            struct timespec time;
        } TICK;
    };
};

int g_events_init(void);
void g_events_done(void);
void g_events_handle(void);
bool g_events_is_empty(void);
void g_events_flush(void);

void g_event_send(
        enum g_sysevent_type type,
        const struct g_sysevent_data * data
);

void g_event_ticktime_set(game_time_ms_t ticktime);

void g_events_pump(void);

bool g_events_event_pump(
        enum g_event_type * type,
        struct g_event_data * data
);

#endif /* SRC_G_EVENTS_H_ */
