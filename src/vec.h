/*
 * vec.h
 *
 *  Created on: 21 авг. 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_VEC_H_
#define SRC_VEC_H_

#define origin_set(origin, x, y) \
        ((origin)->x = (x), (origin)->y = (y))

#define origins_eq(origin1, origin2) \
        ((origin1)->x == (origin2)->x && ((origin1)->y == (origin2)->y))

typedef int vec_t;


typedef struct
{
    vec_t x;
    vec_t y;
} origin_t;

#endif /* SRC_VEC_H_ */
