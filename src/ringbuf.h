#ifndef RINGBUF_H_
#define RINGBUF_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdbool.h>

typedef struct
{
    /*
     * | empty | 11111 | 22222 | 33333 | empty | empty |
     *           head                    tail
     * len = 3
     * or:
     * | 33333 | 44444 | empty | empty | 11111 | 22222 |
     *                   tail            head
     * len = 4
     */
    size_t size;
    size_t len;
    size_t head;
    size_t tail;
} ringbuf_t;

void ringbuf_init(ringbuf_t * ringbuf, size_t size);

void ringbuf_flush(ringbuf_t * ringbuf);

/** @brief Do not move the data */
#define RINGBUF_RESIZE_NO_MOVE 0
/** @brief Move the data [head; size) */
#define RINGBUF_RESIZE_MOVE_RANGE_HEAD_SIZE 1

/**
 * @return res
 * @return      < 0 - Error
 * @return      == RINGBUF_RESIZE_* - What we should make with data in buffer.
 */
int ringbuf_resize(ringbuf_t * ringbuf_new, const ringbuf_t * ringbuf, size_t size);

static inline bool ringbuf_check_empty(const ringbuf_t * ringbuf)
{
    return (ringbuf->len == 0);
}

static inline bool ringbuf_check_full(const ringbuf_t * ringbuf)
{
    return (ringbuf->len >= ringbuf->size);
}

static inline size_t ringbuf_size_get(const ringbuf_t * ringbuf)
{
    return ringbuf->size;
}

static inline size_t ringbuf_length_get(const ringbuf_t * ringbuf)
{
    return ringbuf->len;
}

/**
 * @brief Get the head index (Index, where will be read element)
 */
static inline size_t ringbuf_head_get(const ringbuf_t * ringbuf)
{
	return ringbuf->head;
}

/**
 * @brief Get the tail index (Index, where will be placed new element)
 */
static inline size_t ringbuf_tail_get(const ringbuf_t * ringbuf)
{
	return ringbuf->tail;
}

/**
 * @brief Enqueue element to the tail
 */
void ringbuf_enqueue_tail(ringbuf_t * ringbuf);

/**
 * @brief Enqueue element to the head
 */
void ringbuf_enqueue_head(ringbuf_t * ringbuf);

/**
 * @brief Dequeue element from the head
 */
void ringbuf_dequeue_head(ringbuf_t * ringbuf);

#endif /* RINGBUF_H_ */

