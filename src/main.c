//25.03.2004
//
//змея
//

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

char sys_progversion[] = "SNAKE ver 1.55 (modif: 03.05.2007 ,create(v0.1b): 25.03.2004)";
char sys_special    [] = "Здесь никогда не будет вашей рекламы";
#define sys_special_len 36
char anti_war[] = "Нет войне! Даешь Rock-N-Roll!";

#define mhdelay

#ifdef mhdelay
#include "_time.h"
#else
#include "dos.h"
#endif

#include "_text.h"
#include "str.h"

#define IMENU_MAIN     0x0000
#define IMENU_NEWGAME0 0x0001
#define IMENU_NEWGAME1 0x0101
#define IMENU_NEWGAME2 0x0201
#define IMENU_CHART    0x0301
#define IMENU_HELP     0x0401
#define IMENU_QUIT 0xFFFF

#define LEVEL_MAX 11
#define CHART_RMAX 10

#define MAP_SX 80
#define MAP_SY 24

#define SCORES_PER_LEVEL 100

#define KB_BACKSPACE 8
#define KB_ENTER 13
#define KB_ESC   27
#define KB_UP    72
#define KB_DN    80
#define KB_LF    75
#define KB_RT    77

enum
{
    snake_up,
    snake_dn,
    snake_lf,
    snake_rt
};

//конопля
#define OBJ_MARIJUANA 0
//посаженая конопля
#define OBJ_MARIJUANAP 1
//слабительное
#define OBJ_PURGEN 2
//дерьмо
#define OBJ_SHIRT  3

typedef unsigned long DWORD;
typedef long SDWORD;
//
//шаблон змеи
//
typedef struct Ssnakepattern{
    char dir;
    int  sx,sy;
    int  *pat;
}Tsnakepattern;
//
//сегмент змеи
//
typedef struct Ssnakeseg{
    struct Ssnakeseg *prev;
    struct Ssnakeseg *next;
    char x,y;
}Tsnakeseg;
//
//змея
//
typedef struct Ssnake{
    Tsnakeseg *H;
    char      lastx,lasty;//прошлое положение хвоста(для стирания следа)
    char      movedir;    //направление движения
    int       level;   //уровень развитости
    char      die;     //умерла?
    DWORD     weight;  //вес змеи
    SDWORD    scores;  //очки
}Tsnake;
//
//объекты на карте
//
typedef struct Sobj{
    struct Sobj *next;
    char x,y;
    char id;
    int timer;
}Tobj;
//
//запись таблицы победителей
//
typedef struct Schartrec{
    char  name[16];
    DWORD weight;
    DWORD scores;
}Tchartrec;
//
//таблица победителей
//
typedef struct Schart{
    FILE *F;
    Tchartrec r[CHART_RMAX];
    short num;//количество записей
}Tchart;


char chars_name[]=
        "!#$%&'()+,-.0123456789;=@"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`"
        "abcdefghijklmnopqrstuvwxyz{}~"
        "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
        "абвгдежзийклмнопрстуфхцчшщъыьэюяЁё";

//patterns
int pt0[3]={1,2,3};
int pt1[7][23]={
        { 1, 2, 3, 4, 0,30,31, 0, 0,40,41, 0,71,72,73,74,75,76, 77, 78,79,80,81},//**** **  ** ***********
        { 0, 0, 0, 5, 0,29,32, 0,38,39,42, 0,70, 0, 0, 0, 0, 0, 96, 95, 0, 0,82},//   * ** *** *     **  *
        { 9, 8, 7, 6, 0,28,33,34,37, 0,43, 0,69,68,67,66,65, 0, 97, 94,93,92,83},//**** **** * ***** *****
        {10,11,12, 0, 0,27, 0,35,36, 0,44, 0,60,61,62,63,64, 0, 98, 99, 0,91,84},//***  * ** * ***** ** **
        { 0, 0,13,14, 0,26, 0, 0, 0, 0,45, 0,59, 0, 0, 0, 0, 0,  0,100, 0,90,85},//  ** *    * *      * **
        {18,17,16,15, 0,25, 0, 0, 0, 0,46, 0,58,57,56,55,54, 0,102,101, 0,89,86},//**** *    * ***** ** **
        {19,20,21,22,23,24, 0, 0, 0, 0,47,48,49,50,51,52,53, 0,103,  0, 0,88,87} //******    ******* *  **
};
int pt2[5][20]={
        { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19, 20},
        {40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22, 21},
        {41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59, 60},
        {80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62, 61},
        {81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100}
};

Tsnakepattern info_snake[3]={
        {snake_rt, 3,1,pt0},
        {snake_dn,23,7,*pt1},
        {snake_dn,20,5,*pt2}
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

Tchart chart;
Tsnake snake;
Tobj   *Hobj;


long timing;//задержка(мс)

void obj_new(char x,char y,char id);
void obj_put(char id);
Tobj *obj_free(Tobj **obj);
void obj_draw();

int kbhit(void)
{
    /* fake func */
    return 0;
}

int getch(void)
{
    /* fake func */
    return 0;
}


//////////////////////////////////////////////////
//добавить сегмент змеи к голове
//вход:
//x         -коорд. X
//y         -коорд. Y
//////////////////////////////////////////////////
void snake_newseg(char x,char y){
    Tsnakeseg *P;
    P = malloc(sizeof(Tsnakeseg));
    P->x  =x;
    P->y  =y;
    P->prev=NULL;
    P->next=snake.H;
    if(snake.H) snake.H->prev=P;
    snake.H=P;
    snake.weight++;
};

//////////////////////////////////////////////////
//инициализация змеи(создать змею по шаблону)
//вход:
//pat  -шаблон
//////////////////////////////////////////////////
void snake_init(Tsnakepattern pat){
    int x,y;
    DWORD count;
    char flag;

    snake.H=NULL;
    snake.movedir=pat.dir;
    snake.level=0;
    snake.die=0;
    snake.weight=0;
    snake.scores=0;

    x=0;
    y=0;
    flag=0;
    //ищем хвост змеи
    while(y<pat.sy && !flag){
        while(x<pat.sx && !flag){
            if(pat.pat[y*pat.sx+x]==1) flag=1;
            else x++;
        };
        if(!flag) y++;
    };

    count=1;
    flag=0;
    //строим змею
    snake.lastx=(MAP_SX-pat.sx)/2+x;
    snake.lasty=(MAP_SY-pat.sy)/2+y;
    snake_newseg(snake.lastx,snake.lasty);
    while(count<MAP_SX*MAP_SY && !flag){
        if(y-1>=0       && pat.pat[(y-1)*pat.sx+x]==count+1){
            count++;y--;
            snake_newseg((MAP_SX-pat.sx)/2+x,(MAP_SY-pat.sy)/2+y);
        }
        else
            if(y+1<pat.sy && pat.pat[(y+1)*pat.sx+x]==count+1){
                count++;y++;
                snake_newseg((MAP_SX-pat.sx)/2+x,(MAP_SY-pat.sy)/2+y);
            }
            else
                if(x-1>=0       && pat.pat[y*pat.sx+(x-1)]  ==count+1){
                    count++;x--;
                    snake_newseg((MAP_SX-pat.sx)/2+x,(MAP_SY-pat.sy)/2+y);
                }
                else
                    if(x+1<pat.sx && pat.pat[y*pat.sx+(x+1)]  ==count+1){
                        count++;x++;
                        snake_newseg((MAP_SX-pat.sx)/2+x,(MAP_SY-pat.sy)/2+y);
                    }
                    else flag=1;
    };
};

/////////////////////////////////////////////////
//удалить змею
/////////////////////////////////////////////////
void snake_done(){
    Tsnakeseg *P;
    while(snake.H){
        P      =snake.H;
        snake.H=snake.H->next;
        free(P);
    };
};

//////////////////////////////////////////////////
//отрисовать змею
//////////////////////////////////////////////////
void snake_draw(){
    Tsnakeseg *P;
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
        };
        text_setch(P->x,P->y+1);
        P=P->next;
    };
    text.c.chr=0x00;
    text_setch(snake.lastx,snake.lasty+1);
};

//////////////////////////////////////////////////
//взяли слабительное
//////////////////////////////////////////////////
void snake_get_slabit(){
    int num=2;
    Tsnakeseg *P;
    P=snake.H;
    while(P->next)P=P->next;
    while(num && P->prev){
        obj_new(P->x,P->y,OBJ_SHIRT);//положим дерьмо на карту
        P=P->prev;
        free(P->next);
        P->next=NULL;
        snake.weight--;
        num--;
    };
};

//////////////////////////////////////////////////
//сожрал дерьмо
//////////////////////////////////////////////////
void snake_get_shirt(){
    snake.scores-=2;
    if(snake.scores<0)snake.scores=0;
};

//////////////////////////////////////////////////
//проверка на касание объекта
//выход:
//*obj  -указатель на объект
//////////////////////////////////////////////////
int snake_obj_check(Tobj **obj){
    (*obj)=Hobj;
    while((*obj)){
        switch(snake.movedir){
            case(snake_up):if(snake.H->x  ==(*obj)->x && snake.H->y-1==(*obj)->y){return(1);}break;
            case(snake_dn):if(snake.H->x  ==(*obj)->x && snake.H->y+1==(*obj)->y){return(1);}break;
            case(snake_lf):if(snake.H->x-1==(*obj)->x && snake.H->y  ==(*obj)->y){return(1);}break;
            case(snake_rt):if(snake.H->x+1==(*obj)->x && snake.H->y  ==(*obj)->y){return(1);}break;
        };
        (*obj)=(*obj)->next;
    };
    return(0);
};

//////////////////////////////////////////////////
//установить новое направление движние
//вход:
//player  - змея
//movedir - направление
//////////////////////////////////////////////////
void player_setdir(Tsnake *player,int movedir){
    Tsnakeseg *neck;//шея змеи :)
    neck=player->H->next;
    switch(movedir){
        case(snake_up):
        case(snake_dn):if(!neck || neck->x!=player->H->x)player->movedir=movedir;break;
        case(snake_lf):
        case(snake_rt):if(!neck || neck->y!=player->H->y)player->movedir=movedir;break;
    };
};

//////////////////////////////////////////////////
//управление змеей
//////////////////////////////////////////////////
void snake_think(){
    Tsnakeseg *p;
    Tsnakeseg *pt;
    Tobj *obj;

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
        };

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
                case(snake_up):snake.H->y--;break;
                case(snake_dn):snake.H->y++;break;
                case(snake_lf):snake.H->x--;break;
                case(snake_rt):snake.H->x++;break;
            };
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
                case(snake_up):snake.H->x=p->x  ;snake.H->y=p->y-1;break;
                case(snake_dn):snake.H->x=p->x  ;snake.H->y=p->y+1;break;
                case(snake_lf):snake.H->x=p->x-1;snake.H->y=p->y  ;break;
                case(snake_rt):snake.H->x=p->x+1;snake.H->y=p->y  ;break;
            };
        };
    };
    //проверка на самопересечение
    pt=snake.H;
    while(pt && !snake.die){
        p=pt->next;
        while(p && !snake.die){
            if(pt->x==p->x && pt->y==p->y) snake.die=1;
            p=p->next;
        };
        pt=pt->next;
    };
    //проверка на выход за границу карты
    if(snake.H->x<0 || MAP_SX<=snake.H->x
            || snake.H->y<0 || MAP_SY<=snake.H->y) snake.die=1;
};

//////////////////////////////////////////////////
//прочитать файл с таблицей
//////////////////////////////////////////////////
void chart_load(){
    chart.num=0;
    if((chart.F=fopen("snake.frg","rb"))!=NULL){ //файл присутствует
        do{
            if(fread(&chart.r[chart.num],sizeof(Tchartrec),1,chart.F)==1) chart.num++;
            else break;
        }while(chart.num<CHART_RMAX);
        fclose(chart.F);
    };
};

//////////////////////////////////////////////////
//запмсать таблицу в файл
//////////////////////////////////////////////////
void chart_save(){
    if(chart.num){
        if((chart.F=fopen("snake.frg","wb"))!=NULL){
            fwrite(chart.r,sizeof(Tchartrec)*chart.num,1,chart.F);
            fclose(chart.F);
        };
    };
};

//////////////////////////////////////////////////
//добавить игрока в таблицу
//////////////////////////////////////////////////
void chart_newrec(Tchartrec *rec){
    short count,c;
    count=0;
    while(count<chart.num && rec->scores<chart.r[count].scores)count++;
    if(count>=CHART_RMAX)return;//не вошли в таблицу
    if(chart.num<CHART_RMAX)chart.num++;
    for(c=chart.num-1;c>count;c--) chart.r[c]=chart.r[c-1];
    chart.r[count]=*rec;
};

//////////////////////////////////////////////////
//попали в таблицу рекордов или нет
//вход:
//rec - данные игрока
//выход:
//0 - не попали
//1 - попали
//////////////////////////////////////////////////
int chart_top(Tchartrec *rec){
    int count=0;
    while(count<chart.num && rec->scores<chart.r[count].scores)count++;
    if(count>=CHART_RMAX)return(0);//не вошли в таблицу
    return(1);
};

//////////////////////////////////////////////////
//добавить объект на карту
//////////////////////////////////////////////////
void obj_new(char x,char y,char id){
    int clean=1;
    Tobj *P;

    if(x<0 || MAP_SX<=x || y<0 || MAP_SY<=y)return;

    P=Hobj;
    while(P && clean){
        clean=(x!=P->x || y!=P->y || id==OBJ_MARIJUANA);
        P=P->next;
    };
    if(!clean)return;

    P=malloc(sizeof(Tobj));
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
    };
};

//////////////////////////////////////////////////
//добавить объект на карту(в случайное место)
//////////////////////////////////////////////////
void obj_put(char id)
{
    Tsnakeseg *Ps;
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
        };
        trycount++;

    }while(!clean || trycount<16);//чобы объект все-таки был создан
    obj_new(x,y,id);
};

//////////////////////////////////////////////////
//удалить все объекты
//////////////////////////////////////////////////
void obj_freeall(){
    Tobj *P;
    while(Hobj){
        P   =Hobj;
        Hobj=Hobj->next;
        free(P);
    };
};

//////////////////////////////////////////////////
//удалить объект
//выход:
//obj -указатель на объект
//////////////////////////////////////////////////
Tobj *obj_free(Tobj **obj){
    Tobj *P;
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
    };
    return((*obj));
};

//////////////////////////////////////////////////
//думалка для объектов
//////////////////////////////////////////////////
void obj_think(){
    Tobj *P;
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
                };

            };
        };
        P=P->next;
    };
};

//////////////////////////////////////////////////
//нарисовать все объекты
//////////////////////////////////////////////////
void obj_draw(){
    Tobj *P;
    P=Hobj;
    while(P){

        switch(P->id){
            case(OBJ_MARIJUANA ):text.c.chr=0x05;break;
            case(OBJ_MARIJUANAP):text.c.chr=0x06;break;
            case(OBJ_PURGEN    ):text.c.chr=0x0B;break;
            case(OBJ_SHIRT     ):text.c.chr='@';break;
        };

        text_setchchr(P->x,P->y+1);
        P=P->next;
    };
};


void menu_snake_die();
/////////////////////////////////////////////////
//игра
/////////////////////////////////////////////////
void game_main(){
    char str[20];
    int key;
    int quit=0;
    int showtiming=0;

    text.c.atr=0x1F;
    text.c.chr=0x00;
    text_fill_screen();


#ifdef mhdelay
    timing=5;
#else
    timing=60;
#endif


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
                    case(KB_UP):player_setdir(&snake,snake_up);break;
                    case(KB_DN):player_setdir(&snake,snake_dn);break;
                    case(KB_LF):player_setdir(&snake,snake_lf);break;
                    case(KB_RT):player_setdir(&snake,snake_rt);
                };
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
                    };
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
#ifdef mhdelay
            if(snake.movedir==snake_up ||
                    snake.movedir==snake_dn) mhtime_delay(timing*3/2);
            else                        mhtime_delay(timing);
#else
            if(snake.movedir==snake_up ||
                    snake.movedir==snake_dn) delay(timing*3/2);
            else                        delay(timing);
#endif
            obj_think();
            obj_draw();
        };

        if(showtiming){
            text_writeATR(0,24,"timing=");
            text_writeATR(7,24,str_WORD2strDEC(str,timing));
            showtiming--;
            if(!showtiming) text_writeATR(0,24,"            ");
        };
    };
    obj_freeall();
};


//////////////////////////////////////////////////
//меню "змеиной смерти"
//////////////////////////////////////////////////
void menu_snake_die(){
    int key;
    int quit=0;
    int c;
    int count;
    char str[6];
    Tchartrec rec;
    text.c.atr=0x0F;
    text_writeATR(32, 3,"Tы типа сдох :-(");
    text.c.atr=0x2F;
    text_writeATR(30, 5,"  ****************  ");
    text_writeATR(30, 6," *                * ");
    text_writeATR(30, 7,"*    **      **    *");
    text_writeATR(30, 8,"*    **      **    *");
    text_writeATR(30, 9,"*        **        *");
    text_writeATR(30,10,"*        **        *");
    text_writeATR(30,11,"*        **        *");
    text_writeATR(30,12,"*        **        *");
    text_writeATR(30,13,"*                  *");
    text_writeATR(30,14,"*     ********     *");
    text_writeATR(30,15,"*   **        **   *");
    text_writeATR(30,16,"*                  *");
    text_writeATR(30,17," **              ** ");
    text_writeATR(30,18,"   **************   ");
    text_writeATR(26,20,"СОЖРАЛ КОНОПЛИ(КГ): ");
    text_writeATR(26+20,20,str_WORD2strDEC(str,snake.scores));

    rec.weight=snake.weight;
    rec.scores=snake.scores;
    rec.name[0]=0x00;

    if(!chart_top(&rec)){//не попали в 10 лучших
        text_writeATR(35,21,"ТЫ ХУДШИЙ!");
        if(!getch())getch();
    }
    else{
        text_writeATR(26,21,"ИМЯ>");

        count=0;
        while(!quit){
            text.c.atr=0x0F;
            c = 0;
            while(c<count && rec.name[c]){
                text.c.chr=rec.name[c];
                text_setch(30+c,21);
                c++;
            };
            text.c.chr=176;
            while(c<16){
                text_setch(30+c,21);
                c++;
            };

            key=getch();
            switch(key){
                case(0x00):getch();break;
                case(KB_BACKSPACE):
             if(count>0){
                 rec.name[count]=0x00;
                 count--;
             }
                break;
                case(KB_ENTER):
             chart_newrec(&rec);
                quit=1;
                break;
                default:{
                    if(count<15 && str_char_find1st(chars_name,key)!=-1){
                        rec.name[count]=key;
                        count++;
                        rec.name[count]=0x00;
                    };
                };
            };//end switch(key)
        };
    };
};

/////////////////////////////////////////////////
//главное меню
/////////////////////////////////////////////////
int menu_main(){
    int key;
    int sub=0;
    text.c.atr=0x00;
    text.c.chr=0x00;
    text_fill_screen();

    text.c.atr=0x09;
    text_writeATR(29,1,"**** **  ** ***********");
    text_writeATR(29,2,"   * ** *** *     **  *");
    text_writeATR(29,3,"**** **** * ***** *****");
    text_writeATR(29,4,"***  * ** * ***** ** **");
    text_writeATR(29,5,"  ** *    * *      * **");
    text_writeATR(29,6,"**** *    * ***** ** **");
    text_writeATR(29,7,"******    ******* *  **");

    text_writeATR(40-(sys_special_len>>1),9,sys_special);

    text_writeATR(1,24,sys_progversion);
    text.c.atr=0x5F;
    text_writeATR((80-10)/2,12," НОВАЯ 1  ");
    text_writeATR((80-10)/2,13," НОВАЯ 2  ");
    text_writeATR((80-10)/2,14," НОВАЯ 3  ");
    text_writeATR((80-10)/2,15,"ПОБЕДИТЕЛИ");
    text_writeATR((80-10)/2,16,"  ПОМОЩЬ  ");
    text_writeATR((80-10)/2,17,"  ВЫХОД   ");
    text.c.atr=0x0F;
    text_writeATR( 7,22,"Mad House Software");
    text_writeATR(10,23,"Programming: Ремнёв Александр a.k.a. MasterSan[MH]");

    for(;;){
        text.c.atr=0x05;
        text_writeATR((80-10)/2-2,12+sub,"->");
        text_writeATR((80-10)/2+10,12+sub,"<-");
        key=getch();
        text_writeATR((80-10)/2-2,12+sub,"  ");
        text_writeATR((80-10)/2+10,12+sub,"  ");
        switch(key){
            case(0x00):
            key=getch();
            switch(key){
                case(KB_UP):if(sub>0) sub--;else sub=5;break;
                case(KB_DN):if(sub<5) sub++;else sub=0;
            };
            break;
                case(KB_ENTER):
            switch(sub){
                case(0):return(IMENU_NEWGAME0);
                case(1):return(IMENU_NEWGAME1);
                case(2):return(IMENU_NEWGAME2);
                case(3):return(IMENU_CHART);
                case(4):return(IMENU_HELP);
                case(5):return(IMENU_QUIT);
            };
                break;
        };
    };
};

/////////////////////////////////////////////////
//новая игра 0
/////////////////////////////////////////////////
int menu_newgame0(){
    snake_init(info_snake[0]);
    game_main();
    snake_done();
    return(IMENU_MAIN);
};

/////////////////////////////////////////////////
//новая игра 1
/////////////////////////////////////////////////
int menu_newgame1(){
    snake_init(info_snake[1]);
    game_main();
    snake_done();
    return(IMENU_MAIN);
};

/////////////////////////////////////////////////
//новая игра 2
/////////////////////////////////////////////////
int menu_newgame2(){
    snake_init(info_snake[2]);
    game_main();
    snake_done();
    return(IMENU_MAIN);
};

int menu_chart(){
    char str[12];
    int count;
    DWORD lev;
    text.c.atr=0x00;
    text.c.chr=0x00;
    text_fill_screen();
    text.c.atr=0x09;
    text_writeATR(20+ 1,7+0,"МЕСТО ИМЯ             ФРАГИ ВЕС   СТАТУС");
    for(count=0;count<chart.num;count++){
        lev=chart.r[count].scores/SCORES_PER_LEVEL;
        if(lev>LEVEL_MAX-1) lev=LEVEL_MAX-1;

        text_writeATR(20+ 1,7+1+count,str_WORD2strDEC(str,count+1));
        text_writeATR(20+ 7,7+1+count,chart.r[count].name);
        text_writeATR(20+23,7+1+count,str_WORD2strDEC(str,chart.r[count].scores));
        text_writeATR(20+29,7+1+count,str_WORD2strDEC(str,chart.r[count].weight));
        text_writeATR(20+35,7+1+count,level[lev]);
    };

    text.c.atr=0x5F;
    text_writeATR((80-29)/2,22,anti_war);
    text.c.atr=0x8F;
    text_writeATR((80-16)/2,23,"PRESS ANY KEY...");
    if(!getch())getch();
    return(IMENU_MAIN);
};

/////////////////////////////////////////////////
//помощь
/////////////////////////////////////////////////
int menu_help(){
    text.c.atr=0x00;
    text.c.chr=0x00;
    text_fill_screen();
    text.c.atr=0x09;

    text_writeATR((80-13)/2,5   ,"ТИПА ЭТО ХЕЛП");
    text_writeATR(20,7+ 0,"Управление:");
    text_writeATR(20,7+ 1,"Стрелки - Указание направления движения");
    text_writeATR(20,7+ 2,"+,-     - Управление скоростью игры");
    text_writeATR(20,7+ 3,"P       - Пауза");
    text_writeATR(20,7+ 4,"ESC     - Выход в меню");
    text_writeATR(20,7+ 6,"Предметы:");
    text_writeATR(20,7+ 7,"\5 - конопля");
    text_writeATR(20,7+ 8,"\6 - выросшая конопля");
    text_writeATR(20,7+ 9,"\13 - пурген");
    text_writeATR(20,7+10,"@ - дерьмо");

    text.c.atr=0x8F;
    text_writeATR((80-16)/2,23,"PRESS ANY KEY...");
    if(!getch())getch();
    return(IMENU_MAIN);
};

int main()
{
    int quit=0;
    int Imenu=IMENU_MAIN;
    char line_st;
    char line_end;
    char cur_x,cur_y;
    text_init80X25X8();
    text.c.atr=0x00;
    text.c.chr=0x00;
    text_fill_screen();
    text_videopage_set(0);
    text_cursor_get(0,&line_st,&line_end,&cur_x,&cur_y);
    text_cursor_setxy(0,80,25);


    srand(time(NULL));

    chart_load();
    while(!quit)
    {
        switch(Imenu)
        {
            case(IMENU_MAIN    ): Imenu = menu_main();break;
            case(IMENU_NEWGAME0): Imenu = menu_newgame0();break;
            case(IMENU_NEWGAME1): Imenu = menu_newgame1();break;
            case(IMENU_NEWGAME2): Imenu = menu_newgame2();break;
            case(IMENU_CHART   ): Imenu = menu_chart();break;
            case(IMENU_HELP    ): Imenu = menu_help();break;
            case(IMENU_QUIT    ): quit = 1;
        };
    };
    chart_save();
    text.c.atr=0x0F;
    text.c.chr=0x00;
    text_fill_screen();
    text_cursor_set(line_st,line_end);
    text_cursor_setxy(0,cur_x,cur_y);
    return(0);
}
