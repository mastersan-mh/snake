/*
 * events.h
 *
 *  Created on: 18 июля 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_G_EVENTS_H_
#define SRC_G_EVENTS_H_

#include <sys/queue.h>

typedef enum
{
    G_EVENT_TICK,
    G_EVENT_KEYBOARD
} event_type_t;

typedef struct
{
    struct
    {
        int key;
    } KEYBOARD;
} event_data_t;


typedef struct event_s
{
    CIRCLEQ_ENTRY(event_s) queue;
    event_type_t type;
    event_data_t data;
} event_t;

typedef CIRCLEQ_HEAD(event_head_s, event_s) event_head_t;

void g_events_init(void);
void g_events_handle(void);
void g_events_flush(void);

void g_event_send(
    event_type_t type,
    const event_data_t * data
);

#endif /* SRC_G_EVENTS_H_ */
