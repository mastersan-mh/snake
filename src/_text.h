/**
 * text
 */
#ifndef TEXT_H_
#define TEXT_H_

/* размер экрана X */
#define TEXT_SCRsx 80
/* размер экрана Y */
#define TEXT_SCRsy 25

#include <curses.h>

/**
 * @brief Print string with it attributes
 * @param[in] x         coord. x
 * @param[in] y         coord. y
 * @param[in] format    string
 */
#define text_print(x, y, format, ...) \
        mvprintw(y, x, format, ##__VA_ARGS__)

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

void text_fill_screen(void);

#endif
