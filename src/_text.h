/**
 * text
 */
#ifndef __TEXT_H
#define __TEXT_H

/* размер экрана X */
#define TEXT_SCRsx 80
/* размер экрана Y */
#define TEXT_SCRsy 25

struct TTEXTchar
{
    unsigned char chr;
    unsigned char atr;
};

struct TTEXT{
    struct TTEXTchar c;
};

extern struct TTEXT text;

void text_init80X25X8(void);

void text_setch(int x, int y);
void text_setchchr(int x, int y);
void text_fill_screen(void);
void text_writeATR(int x, int y, const char *s);

#endif
