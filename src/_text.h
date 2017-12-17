/****************************************************************************/
/*                                                                          */
/*        Модуль работы с текстовым режимом int 10h fn 3h REAL MODE         */
/*                                                                          */
/*        2005 MH Software(r) Corporation                                   */
/*        Author: Master San[MH]                                            */
/*                                                                          */
/****************************************************************************/
#ifndef __TEXT_H
#define __TEXT_H

/* размер экрана X */
#define TEXT_SCRsx 80
/* размер экрана Y */
#define TEXT_SCRsy 25

struct TTEXTchar
{
	char chr;
	char atr;
};

struct TTEXTwin{
	char           x0,y0;          //координаты левого верхнего угла окна
	char           x1,y1;          //координаты правого нижнего угла окна
};

struct TTEXT{
	struct TTEXTchar c;
	struct TTEXTwin WIN;
};

extern struct TTEXT text;

void text_init80X25X8();
void text_videopage_set(char page);
void text_blink(char on);
void text_cursor_set(char line_st,char line_end);
void text_cursor_get(char page,char *line_st,char *line_end,char *x,char *y);
void text_cursor_setxy(char page,char x,char y);

void text_BUFget(short x,short y,short sx,short sy,short *buf);
void text_BUFput(short x,short y,short sx,short sy,short *buf);
void text_BUFgetatr(short x,short y,short sx,short sy,char *buf);
void text_BUFputatr(short x,short y,short sx,short sy,char *buf);
void text_setch(char x,char y);
void text_setchchr(char x,char y);
void text_setchatr(char x,char y);
void text_fill_screen();
void text_fill_row(short x0,short x1,short y);
void text_fill_col(short x,short y0,short y1);
void text_write(char x,char y,char *s);
void text_writeATR(char x,char y,char *s);

void text_WINsetch(char x,char y);
void text_WINsetchchr(char x,char y);
void text_WINfill();
void text_WINwrite(char x,char y,char *s);

#endif
