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
    G_SYSEVENT_TICK     , /**< game tick */
    G_SYSEVENT_VID_WINCH, /**< changing window size */
    G_SYSEVENT_KEYBOARD , /**< keyboard */
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
            struct timespec time;
        } TICK;

        struct
        {
            /* empty */
        } VID_WINCH;

        struct
        {
            int key;
        } KEYBOARD;
    };
};

int g_events_init(void);
void g_events_done(void);
void g_events_handle(void);
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

uint16_t g_events_random(void);

#endif /* SRC_G_EVENTS_H_ */
