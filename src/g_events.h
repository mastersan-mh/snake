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

#include <sys/queue.h>

typedef enum
{
    /* changing window size */
    G_EVENT_VID_WINCH,
    /* keyboard */
    G_EVENT_KEYBOARD,
    /* game tick */
    G_EVENT_TICK,
    /* stop game ticks */
    G_EVENT_STOP_GAME_TICKS
} event_type_t;

typedef union
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
} event_data_t;


typedef struct event_s
{
    CIRCLEQ_ENTRY(event_s) queue;
    event_type_t type;
    event_data_t data;
} event_t;

typedef CIRCLEQ_HEAD(event_head_s, event_s) event_head_t;

int g_events_init(void);
void g_events_done(void);
void g_events_handle(void);
bool g_events_is_empty(void);
void g_events_flush(void);

void g_event_send(
    event_type_t type,
    const event_data_t * data
);

void g_event_ticktime_set(game_time_ms_t ticktime);

void g_events_pump(void);

#endif /* SRC_G_EVENTS_H_ */
