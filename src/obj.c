/*
 * obj.c
 *
 *  Created on: 25 дек. 2017 г.
 *      Author: mastersan
 */

#include "snaketypes.h"
#include "game.h"
#include "obj.h"

#include "fakefunc.h"
#include "_text.h"
#include "_time.h"
#include "str.h"

#include <stdlib.h>

static obj_t *Hobj = NULL;
static snake_t snake;

static void snake_think(void);
static void snake_draw();

//////////////////////////////////////////////////
//добавить объект на карту
//////////////////////////////////////////////////
void obj_new(int x, int y, int id)
{
    int clean=1;
    obj_t *P;

    if(x < 0 || MAP_SX <= x || y < 0 || MAP_SY <= y)
        return;

    P=Hobj;
    while(P && clean){
        clean=(x!=P->x || y!=P->y || id==OBJ_MARIJUANA);
        P=P->next;
    }
    if(!clean)return;

    P=malloc(sizeof(obj_t));
    P->next=Hobj;
    Hobj=P;

    Hobj->x=x;
    Hobj->y=y;
    Hobj->id=id;
    switch(id){
        case(OBJ_MARIJUANA ):Hobj->timer=-1;break;
        case(OBJ_MARIJUANAP):Hobj->timer=160+80;break;
        case(OBJ_PURGEN    ):Hobj->timer=80;break;
        case(OBJ_SHIRT     ):Hobj->timer=160;break;
    }
}

//////////////////////////////////////////////////
//удалить все объекты
//////////////////////////////////////////////////
void obj_freeall(void)
{
    obj_t *P;
    while(Hobj){
        P   =Hobj;
        Hobj=Hobj->next;
        free(P);
    }
}

//////////////////////////////////////////////////
//удалить объект
//выход:
//obj -указатель на объект
//////////////////////////////////////////////////
obj_t *obj_free(obj_t **obj)
{
    obj_t *P;
    if(!Hobj || !obj){
        (*obj)=NULL;
        return(NULL);
    }

    text.c.chr=' ';
    text_setchchr((*obj)->x,(*obj)->y+1);

    if(Hobj==(*obj)){
        Hobj=Hobj->next;
        free((*obj));
        (*obj)=Hobj;
    }
    else{
        P=Hobj;
        while(P->next!=(*obj))P=P->next;
        P->next=(*obj)->next;
        free((*obj));
        (*obj)=P;
    }
    return((*obj));
}

//////////////////////////////////////////////////
//добавить объект на карту(в случайное место)
//////////////////////////////////////////////////
void obj_put(int id)
{
    snake_seg_t *Ps;
    int x;
    int y;
    bool clean;//можно создавать объект
    int trycount=0;

    do{

        x=rand()%MAP_SX;
        y=rand()%MAP_SY;
        if(x<0 || MAP_SX<=x || y<0 || MAP_SY<=y)continue;

        //проверим попадание на змеюку
        Ps = snake.H;
        clean = true;
        while(Ps && clean)
        {
            clean = (x != Ps->x || y != Ps->y);
            Ps = Ps->next;
        }
        trycount++;

    }while(clean != true && trycount < 16);//чобы объект все-таки был создан
    obj_new(x,y,id);
}

//////////////////////////////////////////////////
//думалка для объектов
//////////////////////////////////////////////////
void obj_think(void)
{
    obj_t *P;
    int x;
    int y;
    int id;

    for(P = Hobj; P; P = P->next)
    {
        if(P->timer ==-1)
        {
            continue;
        }
        P->timer--;
        if(P->timer > 0)
        {
            continue;
        }

        x = P->x;
        y = P->y;
        id = P->id;
        obj_free(&P);
        switch(id)
        {
            case OBJ_MARIJUANA :break;
            case OBJ_MARIJUANAP:break;
            case OBJ_PURGEN    :break;
            case OBJ_SHIRT     :
            {
                obj_new(x-1,y  ,OBJ_MARIJUANAP);
                obj_new(x  ,y-1,OBJ_MARIJUANAP);
                obj_new(x+1,y  ,OBJ_MARIJUANAP);
                obj_new(x  ,y+1,OBJ_MARIJUANAP);
                obj_new(x  ,y  ,OBJ_MARIJUANAP);
                break;
            }
        }
    }

    snake_think();
}

//////////////////////////////////////////////////
//нарисовать все объекты
//////////////////////////////////////////////////
void obj_draw(void)
{
    obj_t *P;
    P=Hobj;
    while(P){

        switch(P->id)
        {
            case(OBJ_MARIJUANA ):text.c.chr=0x05;break;
            case(OBJ_MARIJUANAP):text.c.chr=0x06;break;
            case(OBJ_PURGEN    ):text.c.chr=0x0B;break;
            case(OBJ_SHIRT     ):text.c.chr='@';break;
        }

        text_setchchr(P->x,P->y+1);
        P=P->next;
    }

    snake_draw();

}


//////////////////////////////////////////////////
//проверка на касание объекта
//выход:
//*obj  -указатель на объект
//////////////////////////////////////////////////
int snake_obj_check(obj_t **obj){
    (*obj)=Hobj;
    while((*obj)){
        switch(snake.movedir){
            case(DIRECTION_NORTH):if(snake.H->x  ==(*obj)->x && snake.H->y-1==(*obj)->y){return(1);}break;
            case(DIRECTION_SOUTH):if(snake.H->x  ==(*obj)->x && snake.H->y+1==(*obj)->y){return(1);}break;
            case(DIRECTION_WEST):if(snake.H->x-1==(*obj)->x && snake.H->y  ==(*obj)->y){return(1);}break;
            case(DIRECTION_EAST):if(snake.H->x+1==(*obj)->x && snake.H->y  ==(*obj)->y){return(1);}break;
        }
        (*obj)=(*obj)->next;
    }
    return(0);
}


//////////////////////////////////////////////////
//добавить сегмент змеи к голове
//вход:
//x         -коорд. X
//y         -коорд. Y
//////////////////////////////////////////////////
void snake_newseg(int  x, int y)
{
    snake_seg_t *P;
    P     = malloc(sizeof(snake_seg_t));
    P->x  =x;
    P->y  =y;
    P->prev=NULL;
    P->next=snake.H;
    if(snake.H) snake.H->prev=P;
    snake.H=P;
    snake.weight++;
}

/**
 * @brief инициализация змеи(создать змею по шаблону)
 * вход:
 * pat  -шаблон
 */
void snake_init(const snake_pattern_t * pat)
{
    int x,y;
    size_t count;
    char flag;

    snake.H = NULL;
    snake.movedir = pat->dir;

    game_delay_update(snake.movedir);

    snake.level = 0;
    snake.dead = 0;
    snake.weight = 0;
    snake.scores = 0;

    x=0;
    y=0;
    flag=0;
    //ищем хвост змеи
    while(y<pat->sy && !flag){
        while(x<pat->sx && !flag){
            if(pat->pat[y * pat->sx + x] == 1)
                flag=1;
            else
                x++;
        }
        if(!flag) y++;
    }

    count=1;
    flag=0;
    //строим змею
    snake.lastx = (MAP_SX - pat->sx) / 2 + x;
    snake.lasty = (MAP_SY - pat->sy) / 2 + y;
    snake_newseg(snake.lastx,snake.lasty);
    while(count<MAP_SX*MAP_SY && !flag){
        if(y-1 >= 0       && pat->pat[(y-1)*pat->sx+x] == count + 1)
        {
            count++;
            y--;
            snake_newseg((MAP_SX-pat->sx)/2+x,(MAP_SY-pat->sy)/2+y);
        }
        else
            if(y+1<pat->sy && pat->pat[(y+1)*pat->sx+x]==count+1){
                count++;y++;
                snake_newseg((MAP_SX-pat->sx)/2+x,(MAP_SY-pat->sy)/2+y);
            }
            else
                if(x-1>=0       && pat->pat[y*pat->sx+(x-1)]  ==count+1){
                    count++;x--;
                    snake_newseg((MAP_SX-pat->sx)/2+x,(MAP_SY-pat->sy)/2+y);
                }
                else
                    if(x+1<pat->sx && pat->pat[y*pat->sx+(x+1)]  ==count+1){
                        count++;x++;
                        snake_newseg((MAP_SX-pat->sx)/2+x,(MAP_SY-pat->sy)/2+y);
                    }
                    else flag=1;
    }
}

/////////////////////////////////////////////////
//удалить змею
/////////////////////////////////////////////////
void snake_done(void)
{
    snake_seg_t *P;
    while(snake.H){
        P      =snake.H;
        snake.H=snake.H->next;
        free(P);
    }
}

//////////////////////////////////////////////////
//отрисовать змею
//////////////////////////////////////////////////
static void snake_draw()
{
    snake_seg_t *P;
    P=snake.H;
    while(P)
    {
        if(snake.dead) text.c.atr=0x44;
        else          text.c.atr=0x1F;
        if(!P->prev) text.c.chr=0x01;//голова
        else{
            if(!P->next) text.c.chr='*';//хвост
            else{                       //тело
                if(P->x==P->next->x && P->x==P->prev->x) text.c.chr=186;
                else
                    if(P->y==P->next->y && P->y==P->prev->y) text.c.chr=205;
                    else
                        if((P->x+1==P->prev->x && P->y  ==P->prev->y && P->x  ==P->next->x && P->y+1==P->next->y)
                                ||(P->x  ==P->prev->x && P->y+1==P->prev->y && P->x+1==P->next->x && P->y  ==P->next->y)
                        )text.c.chr=201;
                        else
                            if((P->x-1==P->prev->x && P->y  ==P->prev->y && P->x  ==P->next->x && P->y+1==P->next->y)
                                    ||(P->x  ==P->prev->x && P->y+1==P->prev->y && P->x-1==P->next->x && P->y  ==P->next->y)
                            )text.c.chr=187;
                            else
                                if((P->x  ==P->prev->x && P->y-1==P->prev->y && P->x+1==P->next->x && P->y  ==P->next->y)
                                        ||(P->x+1==P->prev->x && P->y  ==P->prev->y && P->x  ==P->next->x && P->y-1==P->next->y)
                                )text.c.chr=200;
                                else
                                    if((P->x  ==P->prev->x && P->y-1==P->prev->y && P->x-1==P->next->x && P->y  ==P->next->y)
                                            ||(P->x-1==P->prev->x && P->y  ==P->prev->y && P->x  ==P->next->x && P->y-1==P->next->y)
                                    )text.c.chr=188;
            }
        }
        text_setch(P->x,P->y+1);
        P=P->next;
    }
    text.c.chr=0x00;
    text_setch(snake.lastx,snake.lasty+1);
}

//////////////////////////////////////////////////
//взяли слабительное
//////////////////////////////////////////////////
void snake_get_slabit(){
    int num=2;
    snake_seg_t *P;
    P=snake.H;
    while(P->next)P=P->next;
    while(num && P->prev){
        obj_new(P->x,P->y,OBJ_SHIRT);//положим дерьмо на карту
        P=P->prev;
        free(P->next);
        P->next=NULL;
        snake.weight--;
        num--;
    }
}

/**
 * сожрал дерьмо
 */
void snake_get_shit()
{
    snake.scores -= 2;
    if(snake.scores<0)
        snake.scores=0;
}

/**
 * управление змеей
 */
static void snake_think(void)
{
    snake_seg_t *p;
    snake_seg_t *pt;
    obj_t *obj;

    snake.level = snake.scores / SCORES_PER_LEVEL;

    if(snake.level > LEVEL_MAX-1)
    {
        snake.level = LEVEL_MAX-1;
    }

    if(snake_obj_check(&obj))
    {//поедание объектов
        switch(obj->id){
            case(OBJ_MARIJUANA)://конопля 8)
            snake.scores++;
            snake_newseg(obj->x,obj->y);
            obj_put(OBJ_MARIJUANA);
            if(rand()%3==1) obj_put(OBJ_PURGEN);
            break;
            case(OBJ_MARIJUANAP)://выросшая конопля
            snake.scores++;
            snake_newseg(obj->x,obj->y);
            break;
            case(OBJ_PURGEN         )://пурген
            snake_get_slabit();
            break;
            case(OBJ_SHIRT          )://дерьмо
            snake_get_shit();
            break;
        }

        obj_free(&obj);
        obj_draw();

        p=snake.H;
        while(p->next) p=p->next;
        snake.lastx=p->x;
        snake.lasty=p->y;
    }
    else
    { /* движение */
        if(!snake.H->next)
        {
            snake.lastx=snake.H->x;
            snake.lasty=snake.H->y;
            switch(snake.movedir){
                case(DIRECTION_NORTH):snake.H->y--;break;
                case(DIRECTION_SOUTH):snake.H->y++;break;
                case(DIRECTION_WEST):snake.H->x--;break;
                case(DIRECTION_EAST):snake.H->x++;break;
            }
        }
        else{         //если более одного сегмента
            p=snake.H;
            while(p->next) p=p->next;
            snake.lastx=p->x;
            snake.lasty=p->y;
            //жопу отрубим...
            p->prev->next=NULL;
            p->prev      =NULL;
            p->next      =snake.H;
            //...и сделаем ее головой
            snake.H->prev=p;
            snake.H      =p;
            p=p->next;//прошлая голова
            switch(snake.movedir){
                case DIRECTION_NORTH: snake.H->x=p->x  ; snake.H->y=p->y-1; break;
                case DIRECTION_SOUTH: snake.H->x=p->x  ; snake.H->y=p->y+1; break;
                case DIRECTION_WEST : snake.H->x=p->x-1; snake.H->y=p->y  ; break;
                case DIRECTION_EAST : snake.H->x=p->x+1; snake.H->y=p->y  ; break;
            }
        }
    }
    //проверка на самопересечение
    pt=snake.H;
    while(pt && !snake.dead){
        p=pt->next;
        while(p && !snake.dead){
            if(pt->x==p->x && pt->y==p->y)
                snake.dead = 1;
            p=p->next;
        }
        pt=pt->next;
    }
    /* проверка на выход за границу карты */
    if(snake.H->x<0 || MAP_SX<=snake.H->x
            || snake.H->y<0 || MAP_SY<=snake.H->y)
        snake.dead = 1;
}

void snake_die(void)
{
    snake.dead = true;
}

bool snake_is_dead(void)
{
    return snake.dead;
}

/**
 * @brief установить новое направление движние
 * @param[in] player    змея
 * @param[in] movedir   направление
 */
void player_setdir(direction_t movedir)
{
    snake_seg_t *neck;//шея змеи :)
    neck = snake.H->next;
    switch(movedir)
    {
        case DIRECTION_NORTH:
        case DIRECTION_SOUTH:
            if(!neck || neck->x!=snake.H->x)
                snake.movedir=movedir;
            break;
        case DIRECTION_WEST:
        case DIRECTION_EAST:
            if(!neck || neck->y!=snake.H->y)
                snake.movedir=movedir;
            break;
    }
}

int player_scores(void)
{
    return snake.scores;
}
int player_level(void)
{
    return snake.level;
}
int player_weight(void)
{
    return snake.weight;
}

