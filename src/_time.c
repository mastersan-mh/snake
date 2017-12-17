/****************************************************************************/
/*                                                                          */
/*        Модуль работы с системным таймером                                */
/*        version 1.00                                                      */
/*                                                                          */
/*        2002, MH Software(r) Corporation                                  */
/*        Author: Master San[MH]                                            */
/*                                                                          */
/****************************************************************************/
#ifndef __TIME_C
#define __TIME_C
#include "_time.h"

Tmhtime mhtime = {};

//////////////////////////////////////////////////////////////////////
//получение системного времени
//////////////////////////////////////////////////////////////////////
void mhtime_get()
{
}

//////////////////////////////////////////////////////////////////////
//установка системного времени
//////////////////////////////////////////////////////////////////////
void mhtime_set()
{
}

//////////////////////////////////////////////////////////////////////
//ожидание
//вход:
//t  -миллисекунды( <=100*60 мсек)
//////////////////////////////////////////////////////////////////////
void mhtime_delay(short t)
{
    if(!t)return;
    int t0, t1;
    char ok;
    mhtime_get();
    t0 = mhtime.ms+100*(mhtime.s);
    do{
        mhtime_get();
        t1=mhtime.ms+100*mhtime.s;
        if(t0 <= t1)
            ok = (t1 - t0 >= t);
        else
            ok = (t1 + 100 * 60 - t0 >= t); //на границе минуты
    }while(!ok);
}

#endif
