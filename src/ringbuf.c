
#include "ringbuf.h"

void ringbuf_init(ringbuf_t * ringbuf, size_t size)
{
    ringbuf->size = size;
    ringbuf->len = 0;
    ringbuf->head = 0;
    ringbuf->tail = 0;
}

void ringbuf_flush(ringbuf_t * ringbuf)
{
    ringbuf->len = 0;
    ringbuf->head = 0;
    ringbuf->tail = 0;
}

int ringbuf_resize(ringbuf_t * ringbuf_new, const ringbuf_t * ringbuf, size_t size)
{
    int res = RINGBUF_RESIZE_NO_MOVE;

    if(ringbuf->len == 0)
    {
        ringbuf_init(ringbuf_new, size);
    }
    else
    {

        if(size > ringbuf->size)
        {
            /* grow */
            ringbuf_new->size = size;
            ringbuf_new->len  = ringbuf->len;

            if(ringbuf->head < ringbuf->tail)
            {
                ringbuf_new->head = ringbuf->head;
                ringbuf_new->tail = ringbuf->tail;
            }
            else /* (ringbuf->head >= ringbuf->tail) */
            {
                if(ringbuf->tail == 0)
                {
                    ringbuf_new->head = ringbuf->head;
                    ringbuf_new->tail = ringbuf->size;
                }
                else
                {
                    /* move the data in range [head; size) */
                    size_t diff = size - ringbuf->size;
                    ringbuf_new->head = ringbuf->head + diff;
                    ringbuf_new->tail = ringbuf->tail;
                    res = RINGBUF_RESIZE_MOVE_RANGE_HEAD_SIZE;
                }
            }

        }
        else if(size < ringbuf->size)
        {
            /* shrink */
            /* TODO: shrink */
            return -1;
        }
        else
        {
            /* Do not change */
            (*ringbuf_new) = (*ringbuf);
        }

    }

    return res;
}

size_t ringbuf_enqueue(ringbuf_t * ringbuf)
{
    size_t index = ringbuf->tail;

    ++(ringbuf->len);

    if(ringbuf->tail == ringbuf->size - 1)
    {
        ringbuf->tail = 0;
    }
    else
    {
        ++(ringbuf->tail);
    }
    return index;
}

size_t ringbuf_enqueue_head(ringbuf_t * ringbuf)
{
    ++(ringbuf->len);

    if(ringbuf->head == 0)
    {
        ringbuf->head = ringbuf->size - 1;
    }
    else
    {
        --(ringbuf->head);
    }

    size_t index = ringbuf->head;

    return index;
}

size_t ringbuf_dequeue(ringbuf_t * ringbuf)
{
    size_t index = ringbuf->head;

    --(ringbuf->len);

    if(ringbuf->head == ringbuf->size - 1)
    {
        ringbuf->head = 0;
    }
    else
    {
        ++(ringbuf->head);
    }
    return index;
}
