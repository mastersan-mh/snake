/*
 * ent_objects.c
 *
 *  Created on: 25 дек. 2017 г.
 *      Author: mastersan
 */

#include "ents_objects.h"

#include "io.h"

#include "Z_mem.h"
#include "g_types.h"
#include "game.h"

#include "sys_time.h"

#include <stdlib.h>
#include "ents_ctrl.h"

#define DRAW_ATR (0x00)

char *level_str[LEVEL_MAX] =
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

/* patterns */
static int pt0[1 * 3] = { 1, 2, 3 };
static int pt1[7 * 23] =
{
         1, 2, 3, 4, 0,30,31, 0, 0,40,41, 0,71,72,73,74,75,76, 77, 78,79,80,81, //**** **  ** ***********
         0, 0, 0, 5, 0,29,32, 0,38,39,42, 0,70, 0, 0, 0, 0, 0, 96, 95, 0, 0,82, //   * ** *** *     **  *
         9, 8, 7, 6, 0,28,33,34,37, 0,43, 0,69,68,67,66,65, 0, 97, 94,93,92,83, //**** **** * ***** *****
        10,11,12, 0, 0,27, 0,35,36, 0,44, 0,60,61,62,63,64, 0, 98, 99, 0,91,84, //***  * ** * ***** ** **
         0, 0,13,14, 0,26, 0, 0, 0, 0,45, 0,59, 0, 0, 0, 0, 0,  0,100, 0,90,85, //  ** *    * *      * **
        18,17,16,15, 0,25, 0, 0, 0, 0,46, 0,58,57,56,55,54, 0,102,101, 0,89,86, //**** *    * ***** ** **
        19,20,21,22,23,24, 0, 0, 0, 0,47,48,49,50,51,52,53, 0,103,  0, 0,88,87  //******    ******* *  **
};

static int pt2[5 * 20] =
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


static obj_t *Hobj = NULL;
static snake_t snake;

static void snake_think(const game_ctx_t * gctx);
static void snake_draw(const game_ctx_t * gctx);

/**
 * @brief create object on the map
 */
void obj_new(int x, int y, obj_type_t objtype)
{
    bool clean = true;
    obj_t *P;

    if(x < 0 || MAP_SX <= x || y < 0 || MAP_SY <= y)
        return;

    P = Hobj;
    while(P != NULL && clean)
    {
        clean = (x != P->x || y != P->y || objtype == OBJ_MARIJUANA);
        P = P->next;
    }
    if(!clean)
        return;

    P = Z_malloc(sizeof(obj_t));
    P->next = Hobj;
    Hobj = P;

    Hobj->x = x;
    Hobj->y = y;
    Hobj->type = objtype;
    switch(objtype)
    {
        case OBJ_MARIJUANA : Hobj->timer = -1; break;
        case OBJ_MARIJUANAP: Hobj->timer = 160+80; break;
        case OBJ_PURGEN    : Hobj->timer = 80; break;
        case OBJ_SHIT      : Hobj->timer = 160; break;
        case OBJ_PLAYER    : Hobj->timer = -1; break;
    }
}

//////////////////////////////////////////////////
//удалить все объекты
//////////////////////////////////////////////////
void obj_freeall(void)
{
    obj_t *P;
    while(Hobj != NULL)
    {
        P    = Hobj;
        Hobj = Hobj->next;
        Z_free(P);
    }
}

//////////////////////////////////////////////////
//удалить объект
//выход:
//obj -указатель на объект
//////////////////////////////////////////////////
obj_t *obj_free(const game_ctx_t * gctx, obj_t **obj)
{
    obj_t *P;
    if(Hobj == NULL || obj == NULL)
    {
        (*obj) = NULL;
        return NULL;
    }

#undef TEXT_ATR
#define TEXT_ATR DRAW_ATR
    gctx->putch((*obj)->x, (*obj)->y+1 , TEXT_ATR, ' ');

    if(Hobj == (*obj))
    {
        Hobj = Hobj->next;
        Z_free((*obj));
        (*obj) = Hobj;
    }
    else
    {
        P = Hobj;
        while(P->next != (*obj))
        {
            P = P->next;
        }
        P->next = (*obj)->next;
        free((*obj));
        (*obj) = P;
    }
    return (*obj);
}

/**
 * @brief Enplace object on map in random place
 */
void obj_put(obj_type_t id)
{
    snake_seg_t *Ps;
    int x;
    int y;
    bool clean;//можно создавать объект
    int trycount=0;

    do{

        x = rand()%MAP_SX;
        y = rand()%MAP_SY;
        if(x < 0 || MAP_SX <= x || y < 0 || MAP_SY <= y)
        {
            continue;
        }

        //проверим попадание на змеюку
        Ps = snake.H;
        clean = true;
        while(Ps != NULL && clean)
        {
            clean = (x != Ps->x || y != Ps->y);
            Ps = Ps->next;
        }
        trycount++;

    }while(clean != true && trycount < 16);//чобы объект все-таки был создан
    obj_new(x, y, id);
}

/**
 * @brief object thinker
 */
void obj_think(const game_ctx_t * gctx)
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
        --P->timer;
        if(P->timer > 0)
        {
            continue;
        }

        x = P->x;
        y = P->y;
        id = P->type;
        obj_free(gctx, &P);
        switch(id)
        {
            case OBJ_MARIJUANA :break;
            case OBJ_MARIJUANAP:break;
            case OBJ_PURGEN    :break;
            case OBJ_SHIT     :
            {
                obj_new(x-1, y  , OBJ_MARIJUANAP);
                obj_new(x  , y-1, OBJ_MARIJUANAP);
                obj_new(x+1, y  , OBJ_MARIJUANAP);
                obj_new(x  , y+1, OBJ_MARIJUANAP);
                obj_new(x  , y  , OBJ_MARIJUANAP);
                break;
            }
        }
    }

    snake_think(gctx);
}

/**
 * @brief Draw all objects
 */
static void obj_draw(const game_ctx_t * gctx)
{
#undef TEXT_ATR
#define TEXT_ATR DRAW_ATR
    obj_t *P;
    P = Hobj;
    while(P != NULL)
    {
        char ch = '\0';
        switch(P->type)
        {
            case OBJ_MARIJUANA : ch = 0x05; break;
            case OBJ_MARIJUANAP: ch = 0x06; break;
            case OBJ_PURGEN    : ch = 0x0B; break;
            case OBJ_SHIT      : ch = '@'; break;
            case OBJ_PLAYER    : ch = 's'; break;
        }

        gctx->putch(P->x, P->y+1, TEXT_ATR, ch);
        P=P->next;
    }

    snake_draw(gctx);


}

static void game_draw_state_run(const game_ctx_t * gctx)
{
#undef TEXT_ART
#define TEXT_ART (0x0F)

    gctx->print( 0, 0, TEXT_ART, " СОЖРАЛ КОНОПЛИ: %6d СТАТУС: %-20s ВАШ ВЕС: %6d "
            , player_scores()
            , player_level()
            , player_weight()
    );

    obj_draw(gctx);

    if(game_ents.showtiming > 0)
    {
        gctx->print(0, 24, TEXT_ART, "timing = %d", (int)game_ents.timing);
        --game_ents.showtiming;
        if(game_ents.showtiming <= 0)
            gctx->print(0, 24, TEXT_ART, "            ");
    }
}


void ent_scene_draw(const game_ctx_t * gctx)
{
    static bool paused_prev = false;

    switch(game_ents.state)
    {
        case GSTATE_START:
            break;
        case GSTATE_STOP_WIN:
            break;
        case GSTATE_STOP_LOSE:
            break;
        case GSTATE_REQUEST_STOP:
        {
#undef TEXT_ATR
#define TEXT_ATR (0x0F)
            gctx->print_centerscreen(16, TEXT_ATR, "УЖЕ УХОДИШ[Y/N]?");
            break;
        }
        case GSTATE_REQUEST_STOP_CANCEL:
        {
#undef TEXT_ATR
#define TEXT_ATR (0x1F)
            gctx->print_centerscreen(16, TEXT_ATR, "                ");
            break;
        }
        case GSTATE_ENDGAME:
        {
            break;
        }
        case GSTATE_RUN:
            if(game_ents.paused != paused_prev)
            {
                paused_prev = game_ents.paused;
                if(game_ents.paused)
                {
#undef TEXT_ATR
#define TEXT_ATR (0x8F)
                    gctx->print_centerscreen(17, TEXT_ATR, "-= P A U S E D =-");
                }
                else
                {
#undef TEXT_ATR
#define TEXT_ATR (0x1F)
                    gctx->print_centerscreen(17, TEXT_ATR, "                 ");
                }
            }
            if(!game_ents.paused)
            {
                game_draw_state_run(gctx);
            }
            break;
    }

}



/**
 * проверка на касание объекта
 * выход:
 * *obj  -указатель на объект
 */
int snake_obj_check(obj_t **obj){
    (*obj)=Hobj;
    while((*obj))
    {
        switch(snake.movedir)
        {
            case DIRECTION_NORTH:
                if(snake.H->x   == (*obj)->x && snake.H->y-1 == (*obj)->y) return 1;
                break;
            case DIRECTION_SOUTH:
                if(snake.H->x   == (*obj)->x && snake.H->y+1 == (*obj)->y) return 1;
                break;
            case DIRECTION_WEST :
                if(snake.H->x-1 == (*obj)->x && snake.H->y   == (*obj)->y) return 1;
                break;
            case DIRECTION_EAST :
                if(snake.H->x+1 == (*obj)->x && snake.H->y   == (*obj)->y) return 1;
                break;
        }
        (*obj)=(*obj)->next;
    }
    return 0;
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
    P     = Z_malloc(sizeof(snake_seg_t));
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
void snake_init(const game_ctx_t * gctx, const snake_pattern_t * pat)
{
    int x,y;
    size_t count;
    char flag;

    snake.H = NULL;
    snake.movedir = pat->dir;

    ents_game_timing_update(gctx, snake.movedir);

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
    while(snake.H)
    {
        P       = snake.H;
        snake.H = snake.H->next;
        free(P);
    }
}

//////////////////////////////////////////////////
//отрисовать змею
//////////////////////////////////////////////////
static void snake_draw(const game_ctx_t * gctx)
{
    int atr;
    int ch;
    snake_seg_t *P;
    P=snake.H;
    while(P)
    {
        if(snake.dead) atr = 0x44;
        else          atr = 0x1F;
        if(!P->prev) ch = 0x01;//голова
        else{
            if(!P->next) ch = '*';//хвост
            else{                       //тело
                if(P->x==P->next->x && P->x==P->prev->x) ch = 186;
                else
                    if(P->y==P->next->y && P->y==P->prev->y) ch = 205;
                    else
                        if((P->x+1==P->prev->x && P->y  ==P->prev->y && P->x  ==P->next->x && P->y+1==P->next->y)
                                ||(P->x  ==P->prev->x && P->y+1==P->prev->y && P->x+1==P->next->x && P->y  ==P->next->y)
                        ) ch = 201;
                        else
                            if((P->x-1==P->prev->x && P->y  ==P->prev->y && P->x  ==P->next->x && P->y+1==P->next->y)
                                    ||(P->x  ==P->prev->x && P->y+1==P->prev->y && P->x-1==P->next->x && P->y  ==P->next->y)
                            ) ch = 187;
                            else
                                if((P->x  ==P->prev->x && P->y-1==P->prev->y && P->x+1==P->next->x && P->y  ==P->next->y)
                                        ||(P->x+1==P->prev->x && P->y  ==P->prev->y && P->x  ==P->next->x && P->y-1==P->next->y)
                                ) ch = 200;
                                else
                                    if((P->x  ==P->prev->x && P->y-1==P->prev->y && P->x-1==P->next->x && P->y  ==P->next->y)
                                            ||(P->x-1==P->prev->x && P->y  ==P->prev->y && P->x  ==P->next->x && P->y-1==P->next->y)
                                    ) ch = 188;
            }
        }
        gctx->putch(P->x, P->y+1, atr, ch);
        P = P->next;
    }
    ch = '\0';
    gctx->putch(snake.lastx, snake.lasty+1, atr, ch);
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
        obj_new(P->x, P->y, OBJ_SHIT);
        P=P->prev;
        free(P->next);
        P->next=NULL;
        snake.weight--;
        num--;
    }
}

/**
 * @brief Eat the shit
 */
void snake_get_shit()
{
    snake.scores -= 2;
    if(snake.scores < 0)
        snake.scores = 0;
}

/**
 * управление змеей
 */
static void snake_think(const game_ctx_t * gctx)
{
    snake_seg_t *p;
    snake_seg_t *pt;
    obj_t *obj;

    snake.level = snake.scores / SCORES_PER_LEVEL;

    if(snake.level >= LEVEL_MAX)
    {
        snake.level = LEVEL_MAX - 1;
    }

    if(snake_obj_check(&obj))
    {
        /* objects eating */
        switch(obj->type)
        {
            case OBJ_MARIJUANA:
                ++snake.scores;
                snake_newseg(obj->x,obj->y);
                obj_put(OBJ_MARIJUANA);
                if(rand()%3==1) obj_put(OBJ_PURGEN);
                break;
            case OBJ_MARIJUANAP:
                /* growed up */
                ++snake.scores;
                snake_newseg(obj->x,obj->y);
                break;
            case OBJ_PURGEN:
                snake_get_slabit();
                break;
            case OBJ_SHIT:
                snake_get_shit();
                break;
            case OBJ_PLAYER:
                break;
        }

        obj_free(gctx, &obj);
        obj_draw(gctx);

        p = snake.H;
        while(p->next)
        {
            p = p->next;
        }
        snake.lastx = p->x;
        snake.lasty = p->y;
    }
    else
    {
        /* moving */
        if(snake.H->next == NULL)
        {
            snake.lastx = snake.H->x;
            snake.lasty = snake.H->y;
            switch(snake.movedir)
            {
                case DIRECTION_NORTH: snake.H->y--;break;
                case DIRECTION_SOUTH: snake.H->y++;break;
                case DIRECTION_WEST : snake.H->x--;break;
                case DIRECTION_EAST : snake.H->x++;break;
            }
        }
        else
        {
            /* more than one segment */
            p = snake.H;
            while(p->next != NULL)
            {
                p = p->next;
            }
            snake.lastx=p->x;
            snake.lasty=p->y;
            /* cut the ass... */
            p->prev->next=NULL;
            p->prev      =NULL;
            p->next      =snake.H;
            /* ... and make it a head */
            snake.H->prev=p;
            snake.H      =p;
            p=p->next; /* previous head */
            switch(snake.movedir)
            {
                case DIRECTION_NORTH: snake.H->x = p->x  ; snake.H->y = p->y-1; break;
                case DIRECTION_SOUTH: snake.H->x = p->x  ; snake.H->y = p->y+1; break;
                case DIRECTION_WEST : snake.H->x = p->x-1; snake.H->y = p->y  ; break;
                case DIRECTION_EAST : snake.H->x = p->x+1; snake.H->y = p->y  ; break;
            }
        }
    }
    /* self-cross check */
    pt = snake.H;
    while(pt && !snake.dead)
    {
        p=pt->next;
        while(p && !snake.dead)
        {
            if(pt->x == p->x && pt->y == p->y)
                snake.dead = true;
            p = p->next;
        }
        pt = pt->next;
    }
    /* проверка на выход за границу карты */
    if(snake.H->x < 0 || MAP_SX <= snake.H->x
            || snake.H->y < 0 || MAP_SY <= snake.H->y)
    {
        snake.dead = true;
    }
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
void player_setdir(ent_direction_t movedir)
{
    snake_seg_t *neck;//шея змеи :)
    if(snake.H == NULL)
    {
        return;
    }
    neck = snake.H->next;
    switch(movedir)
    {
        case DIRECTION_NORTH:
        case DIRECTION_SOUTH:
            if(!neck || neck->x!=snake.H->x)
                snake.movedir = movedir;
            break;
        case DIRECTION_WEST:
        case DIRECTION_EAST:
            if(!neck || neck->y!=snake.H->y)
                snake.movedir = movedir;
            break;
    }
}

ent_direction_t player_direction(void)
{
    return snake.movedir;
}

int player_scores(void)
{
    return snake.scores;
}

const char * player_level(void)
{
    return level_str[snake.level];
}

int player_weight(void)
{
    return snake.weight;
}

