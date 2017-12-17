/****************************************************************************
 *                                                                          *
 *        Модуль работы с текстовым режимом int 10h fn 3h REAL MODE         *
 *                                                                          *
 *        2005 MH Software(r) Corporation                                   *
 *        Author: Master San[MH]                                            *
 *                                                                          *
 ****************************************************************************/
#ifndef TEXT_C
#define TEXT_C

#include "_text.h"

struct TTEXT text = {};

/**
 * инициализация текстового режима
 */
void text_init80X25X8(){
    text.c.chr=0x00;
    text.c.atr=0x00;
    text.WIN.x0=0;
    text.WIN.y0=0;
    text.WIN.x1=TEXT_SCRsx-1;
    text.WIN.y1=TEXT_SCRsy-1;
}

/**
 * переключить видеостраницу
 * вход:
 * page      -номер активной страницы[0..7]
 */
void text_videopage_set(char page)
{
}

/**
 * включить/выключить бит мигания
 * вход:
 * on       -вкл/выкл (0/1)
 */
void text_blink(char on)
{
}

/**
 * установка типа курсора
 * вход:
 * line_st       -строка начала курсора(значимые биты 0-4)
 * line_end      -строка конца курсора(значимые биты 0-4)
 */
void text_cursor_set(char line_st, char line_end)
{
    line_st  &= 31;//значимые биты 0-4
    line_end &= 31;//значимые биты 0-4
}

/**
 * установка позиции курсора
 * вход:
 * page          -видеостраница
 * x             -столбец
 * y             -строка
 */
void text_cursor_setxy(char page,char x,char y)
{
}

/**
 * получить состояние курсора
 * вход:
 * page          -видеостраница
 * выход:
 * line_st       -строка начала курсора(значимые биты 0-4)
 * line_end      -строка конца курсора(значимые биты 0-4)
 * x             -столбец
 * y             -строка
 */
void text_cursor_get(char page,char *line_st,char *line_end,char *x,char *y)
{
}

/**
 * сохранение содержимого экрана(символы и атрибуты)
 * примечание:
 * нет проверки на выход за границу экрана
 * вход:
 * x             -координата X
 * y             -координата Y
 * sx            -размер области по OX
 * sy            -размер области по OY
 * buf           -указатель буфера сохранения
 */
void text_BUFget(short x,short y,short sx,short sy,short *buf)
{
}

/**
 * вывод содержимого buf на экран(символы и атрибуты)
 * примечание:
 * нет проверки на выход за границу экрана
 * вход:
 * x             -координата X
 * y             -координата Y
 * sx            -размер области по OX
 * sy            -размер области по OY
 * buf           -указатель буфера сохранения
 */
void text_BUFput(short x,short y,short sx,short sy,short *buf)
{
}

/**
 * сохранение содержимого экрана(атрибуты)
 * примечание:
 * нет проверки на выход за границу экрана
 * вход:
 * x             -координата X
 * y             -координата Y
 * sx            -размер области по OX
 * sy            -размер области по OY
 * buf           -указатель буфера сохранения
 */
void text_BUFgetatr(short x,short y,short sx,short sy,char *buf)
{
}

/**
 * вывод содержимого buf на экран(атрибуты)
 * примечание:
 * нет проверки на выход за границу экрана
 * вход:
 * x             -координата X
 * y             -координата Y
 * sx            -размер области по OX
 * sy            -размер области по OY
 * buf           -указатель буфера сохранения
 */
void text_BUFputatr(short x,short y,short sx,short sy,char *buf)
{
}

/**
 * поместить символ с атрибутами на экран(с клиппингом)
 * вход:
 * x             -координата X
 * y             -координата Y
 */
void text_setch(char x,char y)
{
}

/**
 * поместить символ на экран(с клиппингом)
 * вход:
 * x             -координата X
 * y             -координата Y
 */
void text_setchchr(char x,char y)
{
}

/**
 * поместить атрибуты cимвола на экран(с клиппингом)
 * вход:
 * x             -координата X
 * y             -координата Y
 */
void text_setchatr(char x,char y)
{
}

/**
 * заполнить экран текущими символами и атрибутами
 */
void text_fill_screen()
{
}

/**
 * вывод строки с текущим символом и атрибутами(с клиппингом)
 * вход:
 * x0            -левая граница
 * x1            -правая граница
 * y             -строка
 */
void text_fill_row(short x0,short x1,short y)
{
}

/**
 * вывод столбца с текущими символами и атрибутами(с клиппингом)
 * вход:
 * x0            -левая граница
 * x1            -правая граница
 * y             -строка
 */
void text_fill_col(short x,short y0,short y1)
{
}

/**
 * вывести на экран ASCIIZ строку
 * вход:
 * x             -координата X
 * y             -координата Y
 * s             -указатель на строку
 */
void text_write(char x,char y,char *s)
{
}

/**
 * вывести на экран ASCIIZ строку с атрибутами
 * вход:
 * x             -координата X
 * y             -координата Y
 * s             -указатель на строку
 */
void text_writeATR(char x,char y,char *s)
{
}

/**
 * поместить символ с атрибутами на экран в окно
 * вход:
 * x             -координата X
 * y             -координата Y
 */
void text_WINsetch(char x,char y)
{
}

/**
 * поместить символ на экран в окно
 * вход:
 * x             -координата X
 * y             -координата Y
 */
void text_WINsetchchr(char x,char y)
{
}

/**
 * заполнить окно текущими символами и атрибутами
 */
void text_WINfill()
{
    char x,y;
    for(y=text.WIN.y0;y<=text.WIN.y1;y++)
        for(x=text.WIN.x0;x<=text.WIN.x1;x++)
            text_WINsetch(x,y);
}

/**
 * вывести в окно ASCIIZ строку
 * вход:
 * x             -координата X
 * y             -координата Y
 * s             -указатель на строку
 */
void text_WINwrite(char x, char y, char *s)
{
    while(*s != '\0')
    {
        text.c.chr = *s;
        text_WINsetch(x, y);
        s++;
        x++;
    }
}

#endif
