/*
 * game.c
 *
 *  Created on: 17 дек. 2017 г.
 *      Author: mastersan
 */

#include "snaketypes.h"
#include "menu.h"

#include "fakefunc.h"
#include "_text.h"
#include "_time.h"
#include "str.h"

#include <stdlib.h>

char valid_chars[] =
        "!#$%&'()+,-.0123456789;=@"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`"
        "abcdefghijklmnopqrstuvwxyz{}~"
        "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
        "абвгдежзийклмнопрстуфхцчшщъыьэюяЁё";

//patterns
int pt0[1 * 3] = {1,2,3};
int pt1[7 * 23] = {
         1, 2, 3, 4, 0,30,31, 0, 0,40,41, 0,71,72,73,74,75,76, 77, 78,79,80,81, //**** **  ** ***********
         0, 0, 0, 5, 0,29,32, 0,38,39,42, 0,70, 0, 0, 0, 0, 0, 96, 95, 0, 0,82, //   * ** *** *     **  *
         9, 8, 7, 6, 0,28,33,34,37, 0,43, 0,69,68,67,66,65, 0, 97, 94,93,92,83, //**** **** * ***** *****
        10,11,12, 0, 0,27, 0,35,36, 0,44, 0,60,61,62,63,64, 0, 98, 99, 0,91,84, //***  * ** * ***** ** **
         0, 0,13,14, 0,26, 0, 0, 0, 0,45, 0,59, 0, 0, 0, 0, 0,  0,100, 0,90,85, //  ** *    * *      * **
        18,17,16,15, 0,25, 0, 0, 0, 0,46, 0,58,57,56,55,54, 0,102,101, 0,89,86, //**** *    * ***** ** **
        19,20,21,22,23,24, 0, 0, 0, 0,47,48,49,50,51,52,53, 0,103,  0, 0,88,87  //******    ******* *  **
};

int pt2[5 * 20] =
{
         1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19, 20,
        40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22, 21,
        41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59, 60,
        80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62, 61,
        81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100
};

snake_pattern_t info_snake[] =
{
        {DIRECTION_EAST , 3 , 1, pt0},
        {DIRECTION_SOUTH, 23, 7, pt1},
        {DIRECTION_SOUTH, 20, 5, pt2}
};

char *level[LEVEL_MAX] =
{
        "Так себе, микробик:)",
        "Червячёк            ",
        "Червяк              ",
        "Большой Червяк      ",
        "Глист               ",
        "Взрослый глист      ",
        "Хобот слоненка      ",
        "Хобот слона         ",
        "Матерый бычий цепень",
        "Великий Шланг       ",
        "Бог шлангообразных  "
};

long timing;//задержка(мс)

chart_t chart;
snake_t snake;
obj_t   *Hobj;

void obj_new(char x,char y,char id);
void obj_put(char id);
obj_t *obj_free(obj_t **obj);
void obj_draw();
void player_setdir(snake_t *player, direction_t movedir);

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
    snake.level = 0;
    snake.die = 0;
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
void snake_draw(){
    snake_seg_t *P;
    P=snake.H;
    while(P){
        if(snake.die) text.c.atr=0x44;
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

//////////////////////////////////////////////////
//сожрал дерьмо
//////////////////////////////////////////////////
void snake_get_shirt(){
    snake.scores-=2;
    if(snake.scores<0)snake.scores=0;
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
//установить новое направление движние
//вход:
//player  - змея
//movedir - направление
//////////////////////////////////////////////////
void player_setdir(snake_t *player, direction_t movedir)
{
    snake_seg_t *neck;//шея змеи :)
    neck=player->H->next;
    switch(movedir){
        case(DIRECTION_NORTH):
        case(DIRECTION_SOUTH):if(!neck || neck->x!=player->H->x)player->movedir=movedir;break;
        case(DIRECTION_WEST):
        case(DIRECTION_EAST):if(!neck || neck->y!=player->H->y)player->movedir=movedir;break;
    }
}

//////////////////////////////////////////////////
//управление змеей
//////////////////////////////////////////////////
void snake_think(){
    snake_seg_t *p;
    snake_seg_t *pt;
    obj_t *obj;

    if(snake_obj_check(&obj)){//поедание объектов
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
            snake_get_shirt();
            break;
        }

        obj_free(&obj);
        obj_draw();

        p=snake.H;
        while(p->next) p=p->next;
        snake.lastx=p->x;
        snake.lasty=p->y;
    }
    else{//движение
        if(!snake.H->next){
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
                case(DIRECTION_NORTH):snake.H->x=p->x  ;snake.H->y=p->y-1;break;
                case(DIRECTION_SOUTH):snake.H->x=p->x  ;snake.H->y=p->y+1;break;
                case(DIRECTION_WEST):snake.H->x=p->x-1;snake.H->y=p->y  ;break;
                case(DIRECTION_EAST):snake.H->x=p->x+1;snake.H->y=p->y  ;break;
            }
        }
    }
    //проверка на самопересечение
    pt=snake.H;
    while(pt && !snake.die){
        p=pt->next;
        while(p && !snake.die){
            if(pt->x==p->x && pt->y==p->y) snake.die=1;
            p=p->next;
        }
        pt=pt->next;
    }
    //проверка на выход за границу карты
    if(snake.H->x<0 || MAP_SX<=snake.H->x
            || snake.H->y<0 || MAP_SY<=snake.H->y) snake.die=1;
}

//////////////////////////////////////////////////
//прочитать файл с таблицей
//////////////////////////////////////////////////
void chart_load(void)
{
    chart.num=0;
    if((chart.F=fopen("snake.frg","rb"))!=NULL){ //файл присутствует
        do{
            if(fread(&chart.r[chart.num],sizeof(chartrec_t),1,chart.F)==1) chart.num++;
            else break;
        }while(chart.num<CHART_RMAX);
        fclose(chart.F);
    }
}

//////////////////////////////////////////////////
//запмсать таблицу в файл
//////////////////////////////////////////////////
void chart_save(void)
{
    if(chart.num){
        if((chart.F=fopen("snake.frg","wb"))!=NULL){
            fwrite(chart.r,sizeof(chartrec_t)*chart.num,1,chart.F);
            fclose(chart.F);
        }
    }
}

//////////////////////////////////////////////////
//добавить игрока в таблицу
//////////////////////////////////////////////////
void chart_newrec(chartrec_t *rec)
{
    short count,c;
    count=0;
    while(count<chart.num && rec->scores<chart.r[count].scores)count++;
    if(count>=CHART_RMAX)return;//не вошли в таблицу
    if(chart.num<CHART_RMAX)chart.num++;
    for(c=chart.num-1;c>count;c--) chart.r[c]=chart.r[c-1];
    chart.r[count]=*rec;
}

//////////////////////////////////////////////////
//попали в таблицу рекордов или нет
//вход:
//rec - данные игрока
//выход:
//0 - не попали
//1 - попали
//////////////////////////////////////////////////
int chart_top(chartrec_t *rec)
{
    int count=0;
    while(count<chart.num && rec->scores<chart.r[count].scores)count++;
    if(count>=CHART_RMAX)return(0);//не вошли в таблицу
    return(1);
}

//////////////////////////////////////////////////
//добавить объект на карту
//////////////////////////////////////////////////
void obj_new(char x, char y, char id){
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
//добавить объект на карту(в случайное место)
//////////////////////////////////////////////////
void obj_put(char id)
{
    snake_seg_t *Ps;
    char x,y;
    char clean;//можно создавать объект
    int trycount=0;

    do{

        x=rand()%MAP_SX;
        y=rand()%MAP_SY;
        if(x<0 || MAP_SX<=x || y<0 || MAP_SY<=y)continue;

        //проверим попадание на змеюку
        clean=1;
        Ps=snake.H;
        while(Ps && clean){
            clean=(x!=Ps->x || y!=Ps->y);
            Ps=Ps->next;
        }
        trycount++;

    }while(!clean || trycount<16);//чобы объект все-таки был создан
    obj_new(x,y,id);
}

//////////////////////////////////////////////////
//удалить все объекты
//////////////////////////////////////////////////
void obj_freeall(){
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
obj_t *obj_free(obj_t **obj){
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
//думалка для объектов
//////////////////////////////////////////////////
void obj_think(){
    obj_t *P;
    char x,y,id;
    P=Hobj;
    while(P){
        if(P->timer!=-1){
            P->timer--;
            if(!P->timer){
                x=P->x;
                y=P->y;
                id=P->id;
                obj_free(&P);
                switch(id){
                    case(OBJ_MARIJUANA ):break;
                    case(OBJ_MARIJUANAP):break;
                    case(OBJ_PURGEN    ):break;
                    case(OBJ_SHIRT     ):
              obj_new(x-1,y  ,OBJ_MARIJUANAP);
                    obj_new(x  ,y-1,OBJ_MARIJUANAP);
                    obj_new(x+1,y  ,OBJ_MARIJUANAP);
                    obj_new(x  ,y+1,OBJ_MARIJUANAP);
                    obj_new(x  ,y  ,OBJ_MARIJUANAP);
                    break;
                }

            }
        }
        P=P->next;
    }
}

//////////////////////////////////////////////////
//нарисовать все объекты
//////////////////////////////////////////////////
void obj_draw(){
    obj_t *P;
    P=Hobj;
    while(P){

        switch(P->id){
            case(OBJ_MARIJUANA ):text.c.chr=0x05;break;
            case(OBJ_MARIJUANAP):text.c.chr=0x06;break;
            case(OBJ_PURGEN    ):text.c.chr=0x0B;break;
            case(OBJ_SHIRT     ):text.c.chr='@';break;
        }

        text_setchchr(P->x,P->y+1);
        P=P->next;
    }
}



/////////////////////////////////////////////////
//игра
/////////////////////////////////////////////////
void game_main(void)
{
    char str[20];
    int key;
    int quit=0;
    int showtiming=0;

    text.c.atr=0x1F;
    text.c.chr=0x00;
    text_fill_screen();


    timing=5;

    obj_put(OBJ_MARIJUANA);
    while(!quit){
        if(kbhit()){
            key = getch();
            switch(key){
                case('P'):case('p'):
             text.c.atr=0x8F;
                text_writeATR((80-13)/2,12,"-=P A U S E=-");
                do{
                    key=getch();
                    if(!key) getch();
                }while(key!='P' && key!='p');
                text.c.atr=0x1F;
                text_writeATR((80-13)/2,12,"             ");
                break;
                case('='):case('+'):if(timing<100)timing++;showtiming=110-timing;break;
                case('-'):case('_'):if(timing>0)timing--;showtiming=110-timing;break;
                case(0x00):
             key = getch();
                switch(key){
                    case(KB_UP):player_setdir(&snake,DIRECTION_NORTH);break;
                    case(KB_DN):player_setdir(&snake,DIRECTION_SOUTH);break;
                    case(KB_LF):player_setdir(&snake,DIRECTION_WEST);break;
                    case(KB_RT):player_setdir(&snake,DIRECTION_EAST);
                }
                break;
                    case(KB_ESC):
             text.c.atr=0x0F;
                    text_writeATR(30,12,"УЖЕ УХОДИШ[Y/N]");
                    do{
                        key = getch();
                        if(!key) getch();
                    }while(!key);
                    if(key=='Y' || key=='y'){//клавиша 'Y'
                        snake.die=1;
                        quit=1;
                    }
                    else{
                        text.c.atr=0x1F;
                        text_writeATR(30,12,"                ");
                    }
                    break;
            }
        }


        snake.level=snake.scores/SCORES_PER_LEVEL;

        if(snake.level>LEVEL_MAX-1) snake.level=LEVEL_MAX-1;

        text.c.atr=0x0F;
        text_writeATR( 0,0," СОЖРАЛ КОНОПЛИ: ");
        text_writeATR(17,0,str_WORD2strDEC(str,snake.scores));
        text_writeATR(28  ,0,"СТАТУС: ");
        text_writeATR(28+8,0,level[snake.level]);
        text_writeATR(64,0," ВАШ ВЕС: ");
        text_writeATR(74,0,str_WORD2strDEC(str,snake.weight));
        text_writeATR(79,0," ");

        if(snake.die){
            menu_snake_die();
            quit=1;
        }
        else{
            snake_think();
            snake_draw();

            if(snake.movedir==DIRECTION_NORTH ||
                    snake.movedir==DIRECTION_SOUTH)
            {
                mhtime_delay(timing*3/2);
            }
            else
            {
                mhtime_delay(timing);
            }
            obj_think();
            obj_draw();
        }

        if(showtiming){
            text_writeATR(0,24,"timing=");
            text_writeATR(7,24,str_WORD2strDEC(str,timing));
            showtiming--;
            if(!showtiming) text_writeATR(0,24,"            ");
        }
    }
    obj_freeall();
}
