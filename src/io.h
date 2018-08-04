/*
 * input.h
 *
 *  Created on: 29 дек. 2017 г.
 *      Author: mastersan
 */

#ifndef SRC_IO_H_
#define SRC_IO_H_

#define IO_KB_NOKEY -1
#define IO_KB_BACKSPACE 263
#define IO_KB_ENTER 10
#define IO_KB_ESC   27
#define IO_KB_UP    259
#define IO_KB_DN    258
#define IO_KB_LF    260
#define IO_KB_RT    261

int io_init(void);
void io_done(void);
int io_render_end(void);

int io_kbhit(void);

int io_getch(void);

#endif /* SRC_IO_H_ */
