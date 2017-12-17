/****************************************************************************/
/*                                                                          */
/*        Модуль работы с системным таймером                                */
/*        version 1.00                                                      */
/*                                                                          */
/*        2002, MH Software(r) Corporation                                  */
/*        Author: Master San[MH]                                            */
/*                                                                          */
/****************************************************************************/
#ifndef __TIME_H
#define __TIME_H

typedef struct Smhtime
{
    unsigned char h ;                      //часы
    unsigned char m ;                      //минуты
    unsigned char s ;                      //секунды
    unsigned char ms;                      //милисекунды
} Tmhtime;

extern Tmhtime mhtime;

void mhtime_get();
void mhtime_set();
void mhtime_delay(short t);

#endif
