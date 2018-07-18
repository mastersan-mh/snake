/*
 * events.c
 *
 *  Created on: 18 июля 2018 г.
 *      Author: mastersan
 */


#include "g_events.h"

#include "Z_mem.h"

#include "game.h"


static event_head_t events;

void g_events_init(void)
{
    CIRCLEQ_INIT(&events);
}

void g_events_handle(void)
{
    while(!CIRCLEQ_EMPTY(&events))
    {
        event_t * event = CIRCLEQ_FIRST(&events);
        CIRCLEQ_REMOVE(&events, event, queue);
        g_fsm(event);
        Z_free(event);
    }
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
