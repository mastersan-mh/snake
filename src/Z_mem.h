/*
 * z_mem.h
 *
 *  Created on: 18 июля 2018 г.
 *      Author: mastersan
 */

#ifndef SRC_Z_MEM_H_
#define SRC_Z_MEM_H_

#include <stdlib.h>
#include <string.h>

#define Z_free(xptr) free(xptr)
#define Z_malloc(xsize) malloc(xsize)
#define Z_calloc(xnmemb, xsize) calloc(xnmemb, xsize)
#define Z_strndup(xstring, xn) strndup(xstring, xn)
#define Z_strdup(xstring) strdup(xstring)


#endif /* SRC_Z_MEM_H_ */
