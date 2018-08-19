/*
 * input.h
 *
 *  Created on: 29 дек. 2017 г.
 *      Author: mastersan
 */

#ifndef SRC_IO_H_
#define SRC_IO_H_

int io_init(void);
void io_done(void);
int io_render_begin(void);
int io_render_end(void);

int io_kbhit(void);

int io_getch(void);

#endif /* SRC_IO_H_ */
