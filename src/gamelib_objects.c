/*
 * gamelib_objects.c
 *
 *  Created on: 25 дек. 2017 г.
 *      Author: mastersan
 */

#include "gamelib_objects.h"
#include "gamelib_ctrl.h"

#include "Z_mem.h"

#include <stdlib.h>

/**
 * @brief Segment of the snake
 */
struct snake_seg
{
    struct snake_seg *prev;
    struct snake_seg *next;

    world_ientity_t ient;
    origin_t origin;
};

/**
 * @brief snake
 */
#define SNAKE_COMMAND_QUEUE_MAX  8

struct snake
{
    struct snake_seg *head;

    ringbuf_t commands_queue;
    enum direction commands[SNAKE_COMMAND_QUEUE_MAX];

    /* movement direction */
    enum direction movedir;
    int       level;   //уровень развитости
    bool      dead;     //умерла?
    long      weight;  //вес змеи
    long      scores;  //очки
};

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
         1, 2, 3, 4, 0,30,31, 0, 0,40,41, 0,71,72,73,74,75,76, 77, 78,79,80,81, /*  **** **  ** ***********  */
         0, 0, 0, 5, 0,29,32, 0,38,39,42, 0,70, 0, 0, 0, 0, 0, 96, 95, 0, 0,82, /*     * ** *** *     **  *  */
         9, 8, 7, 6, 0,28,33,34,37, 0,43, 0,69,68,67,66,65, 0, 97, 94,93,92,83, /*  **** **** * ***** *****  */
        10,11,12, 0, 0,27, 0,35,36, 0,44, 0,60,61,62,63,64, 0, 98, 99, 0,91,84, /*  ***  * ** * ***** ** **  */
         0, 0,13,14, 0,26, 0, 0, 0, 0,45, 0,59, 0, 0, 0, 0, 0,  0,100, 0,90,85, /*    ** *    * *      * **  */
        18,17,16,15, 0,25, 0, 0, 0, 0,46, 0,58,57,56,55,54, 0,102,101, 0,89,86, /*  **** *    * ***** ** **  */
        19,20,21,22,23,24, 0, 0, 0, 0,47,48,49,50,51,52,53, 0,103,  0, 0,88,87  /*  ******    ******* *  **  */
};

static int pt2[5 * 20] =
{
         1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19, 20,
        40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22, 21,
        41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59, 60,
        80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62, 61,
        81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100
};

struct snake_pattern info_snake[] =
{
        {DIRECTION_EAST , 3 , 1, pt0},
        {DIRECTION_SOUTH, 23, 7, pt1},
        {DIRECTION_SOUTH, 20, 5, pt2}
};

static struct obj_st *Hobj = NULL;
static struct snake snake;

/**
 * @brief create object on the map
 */
void obj_new(int x, int y, enum obj_type objtype)
{
    int res;
    bool clean = true;
    struct obj_st *obj;

    if(x < 0 || MAP_SX <= x || y < 0 || MAP_SY <= y)
        return;

    obj = Hobj;

    origin_t orig;
    origin_set(&orig, x, y);

    while(obj != NULL && clean)
    {
        clean = (!origins_eq(&orig, &obj->origin) || objtype == OBJ_MARIJUANA);
        obj = obj->next;
    }
    if(!clean)
        return;

    world_ientity_t ient;
    res = gamelib.geng->world_find_first_free(&ient);
    if(res)
    {
        return;
    }

    obj = Z_malloc(sizeof(struct obj_st));
    if(obj == NULL)
    {
        return;
    }
    obj->next = Hobj;
    Hobj = obj;

    enum model_index model_index = -1;
    switch(objtype)
    {
        case OBJ_MARIJUANA : model_index = MDL_MARIJUANA ; break;
        case OBJ_MARIJUANAP: model_index = MDL_MARIJUANAP; break;
        case OBJ_PURGEN    : model_index = MDL_PURGEN    ; break;
        case OBJ_SHIT      : model_index = MDL_SHIT      ; break;
    }

    obj->ient = ient;
    size_t imodel = gamelib.mdlidx[model_index];

    origin_set(&obj->origin, x, y);

    gamelib.geng->world_ent_update_orig(ient, &obj->origin);
    gamelib.geng->world_ent_update_model(ient, imodel);
    gamelib.geng->world_ent_link(ient);

    obj->type = objtype;
    switch(objtype)
    {
        case OBJ_MARIJUANA : obj->timer = -1; break;
        case OBJ_MARIJUANAP: obj->timer = 160+80; break;
        case OBJ_PURGEN    : obj->timer = 80; break;
        case OBJ_SHIT      : obj->timer = 160; break;
    }
}

/**
 * @brief Erase all objects
 */
void obj_freeall(void)
{
    struct obj_st *obj;
    while(Hobj != NULL)
    {
        obj    = Hobj;
        Hobj = Hobj->next;
        gamelib.geng->world_ent_unlink(obj->ient);
        Z_free(obj);
    }
}

/**
 * @brief Erase object
 * param[in/out] obj    object
 */
/**
 * @brief Erase object
 * param[in/out] obj    object
 */
void obj_free(struct obj_st **obj)
{
    struct obj_st *P;
    if(Hobj == NULL || obj == NULL)
    {
        return;
    }

    if(Hobj == (*obj))
    {
        Hobj = Hobj->next;
        gamelib.geng->world_ent_unlink((*obj)->ient);
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
        gamelib.geng->world_ent_unlink((*obj)->ient);
        Z_free((*obj));
        (*obj) = P;
    }
}

/**
 * @brief Enplace object on map in random place
 */
void obj_put(enum obj_type objtype)
{
    struct snake_seg * snakeseg;
    int x;
    int y;
    bool clean; /* the place is clean */
    int trycount=0;

    do{

        x = rand()%MAP_SX;
        y = rand()%MAP_SY;

        origin_t origin;
        origin_set(&origin, x, y);

        if(x < 0 || MAP_SX <= x || y < 0 || MAP_SY <= y)
        {
            continue;
        }

        //проверим попадание на змеюку
        snakeseg = snake.head;
        clean = true;
        while(snakeseg != NULL && clean)
        {
            clean = !origins_eq(&origin, &snakeseg->origin);
            snakeseg = snakeseg->next;
        }
        trycount++;

    }while(clean != true && trycount < 16);//чобы объект все-таки был создан
    obj_new(x, y, objtype);
}

/**
 * @brief object thinker
 */
void obj_think(void)
{
    struct obj_st *obj;
    int x;
    int y;
    int id;

    for(obj = Hobj; obj; obj = obj->next)
    {
        if(obj->timer ==-1)
        {
            continue;
        }
        --obj->timer;
        if(obj->timer > 0)
        {
            continue;
        }

        x = obj->origin.x;
        y = obj->origin.y;
        id = obj->type;
        obj_free(&obj);
        switch(id)
        {
            case OBJ_MARIJUANA :break;
            case OBJ_MARIJUANAP:break;
            case OBJ_PURGEN    :break;
            case OBJ_SHIT      :
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
}

void gamelib_HUD_draw(void)
{
#undef TEXT_ART
#define TEXT_ART (0x0F)

    gamelib.geng->print( 0, 0, TEXT_ART, " СОЖРАЛ КОНОПЛИ: %6d СТАТУС: %-20s ВАШ ВЕС: %6d "
            , player_scores()
            , player_level()
            , player_weight()
    );

    if(gamelib.showtiming > 0)
    {
        gamelib.geng->print(0, 24, TEXT_ART, "timing = %d", (int)gamelib.timing);
        --gamelib.showtiming;
    }
}

/**
 * проверка на касание объекта
 * выход:
 * *obj  -указатель на объект
 */
static struct obj_st * P_snake_obj_check(void)
{
    struct obj_st * obj = Hobj;
    while(obj)
    {
        switch(snake.movedir)
        {
            case DIRECTION_NORTH:
                if(snake.head->origin.x     == obj->origin.x && snake.head->origin.y - 1 == obj->origin.y) return obj;
                break;
            case DIRECTION_SOUTH:
                if(snake.head->origin.x     == obj->origin.x && snake.head->origin.y + 1 == obj->origin.y) return obj;
                break;
            case DIRECTION_WEST :
                if(snake.head->origin.x - 1 == obj->origin.x && snake.head->origin.y     == obj->origin.y) return obj;
                break;
            case DIRECTION_EAST :
                if(snake.head->origin.x + 1 == obj->origin.x && snake.head->origin.y     == obj->origin.y) return obj;
                break;
        }
        obj = obj->next;
    }
    return NULL;
}

/**
 * @brief Get the model of the snake segment
 */
static size_t P_snake_seg_model_get(const struct snake_seg *sseg)
{
    /* head */
    if(!sseg->prev) return gamelib.mdlidx[MDL_SNAKE_HEAD];
    /* tail */
    if(!sseg->next) return gamelib.mdlidx[MDL_SNAKE_TAIL];

    /* body */
    if(sseg->origin.x == sseg->next->origin.x && sseg->origin.x == sseg->prev->origin.x) return gamelib.mdlidx[MDL_SNAKE_BODY_VERTICAL];
    if(sseg->origin.y == sseg->next->origin.y && sseg->origin.y == sseg->prev->origin.y) return gamelib.mdlidx[MDL_SNAKE_BODY_HORIZONTAL];

    if(
            (  sseg->origin.x + 1 == sseg->prev->origin.x && sseg->origin.y     == sseg->prev->origin.y && sseg->origin.x     == sseg->next->origin.x && sseg->origin.y + 1 == sseg->next->origin.y)
            ||(sseg->origin.x     == sseg->prev->origin.x && sseg->origin.y + 1 == sseg->prev->origin.y && sseg->origin.x + 1 == sseg->next->origin.x && sseg->origin.y     == sseg->next->origin.y)
    )
        return gamelib.mdlidx[MDL_SNAKE_BODY_CORNER_LEFT_UP];

    if(
            (  sseg->origin.x - 1 == sseg->prev->origin.x && sseg->origin.y     == sseg->prev->origin.y && sseg->origin.x     == sseg->next->origin.x && sseg->origin.y + 1 == sseg->next->origin.y)
            ||(sseg->origin.x     == sseg->prev->origin.x && sseg->origin.y + 1 == sseg->prev->origin.y && sseg->origin.x - 1 == sseg->next->origin.x && sseg->origin.y     == sseg->next->origin.y)
    )
        return gamelib.mdlidx[MDL_SNAKE_BODY_CORNER_RIGHT_UP];

    if(
            (  sseg->origin.x     == sseg->prev->origin.x && sseg->origin.y - 1 == sseg->prev->origin.y && sseg->origin.x + 1 == sseg->next->origin.x && sseg->origin.y     == sseg->next->origin.y)
            ||(sseg->origin.x + 1 == sseg->prev->origin.x && sseg->origin.y     == sseg->prev->origin.y && sseg->origin.x     == sseg->next->origin.x && sseg->origin.y - 1 == sseg->next->origin.y)
    )
        return gamelib.mdlidx[MDL_SNAKE_BODY_CORNER_LEFT_DOWN];

    if(
            (  sseg->origin.x     == sseg->prev->origin.x && sseg->origin.y - 1 == sseg->prev->origin.y && sseg->origin.x - 1 == sseg->next->origin.x && sseg->origin.y     == sseg->next->origin.y)
            ||(sseg->origin.x - 1 == sseg->prev->origin.x && sseg->origin.y     == sseg->prev->origin.y && sseg->origin.x     == sseg->next->origin.x && sseg->origin.y - 1 == sseg->next->origin.y)
    )
        return gamelib.mdlidx[MDL_SNAKE_BODY_CORNER_RIGHT_DOWN];
    return -1;
}

static void P_snake_seg_model_update(struct snake_seg *sseg)
{
    struct snake_seg *sseg_iter;

    int iskin = (snake.dead ? 1 : 0);
    /* update the model of the new segment and previous segment */
    int i;
    for(sseg_iter = sseg, i = 0; sseg_iter != NULL && i < 2; sseg_iter = sseg_iter->next, ++i)
    {
        size_t imodel = P_snake_seg_model_get(sseg_iter);
        gamelib.geng->world_ent_update_model(sseg_iter->ient, imodel);
        gamelib.geng->world_ent_update_skin(sseg->ient, iskin);
    }
}

/**
 * @brief Create the new segment at the head
 * @param x
 * @param y
 */
static void P_snake_newseg(vec_t x, vec_t y)
{
    int res;
    struct snake_seg *sseg;
    world_ientity_t ient;
    res = gamelib.geng->world_find_first_free(&ient);
    if(res) return;
    sseg = Z_malloc(sizeof(struct snake_seg));

    sseg->prev = NULL;
    sseg->next = snake.head;
    if(snake.head) snake.head->prev = sseg;
    snake.head = sseg;

    origin_set(&sseg->origin, x, y);
    sseg->ient = ient;

    gamelib.geng->world_ent_update_orig(ient, &sseg->origin);
    gamelib.geng->world_ent_link(ient);

    P_snake_seg_model_update(sseg);


    ++snake.weight;
}

/**
 * @brief инициализация змеи(создать змею по шаблону)
 * вход:
 * pat  -шаблон
 */
void snake_init(const struct snake_pattern * pat)
{
    int x,y;
    size_t index;
    char flag;

    snake.head = NULL;

    ringbuf_init(&snake.commands_queue, SNAKE_COMMAND_QUEUE_MAX);

    snake.movedir = pat->dir;

    ents_game_timing_update(snake.movedir);

    snake.level = 0;
    snake.dead = 0;
    snake.weight = 0;
    snake.scores = 0;

#define OFFSET(pattern, x, y) \
        ((y) * (pat)->sx + (x))

    x=0;
    y=0;
    flag=0;
    /* Find the snake tail */
    index = 1;

    while(y<pat->sy && !flag){
        while(x<pat->sx && !flag){
            if(pat->pat[OFFSET(pat, x, y)] == index)
                flag=1;
            else
                x++;
        }
        if(!flag) y++;
    }

    int edge_left = (MAP_SX - pat->sx) / 2;
    int edge_up   = (MAP_SY - pat->sy) / 2;

    /* build the snake */
    index++;
    P_snake_newseg(edge_left + x, edge_up + y);
    while(index < (MAP_SX * MAP_SY))
    {
        if     (y - 1 >= 0       && pat->pat[OFFSET(pat, x    , y - 1)] == index) y--;
        else if(y + 1 <  pat->sy && pat->pat[OFFSET(pat, x    , y + 1)] == index) y++;
        else if(x - 1 >= 0       && pat->pat[OFFSET(pat, x - 1, y    )] == index) x--;
        else if(x + 1 <  pat->sx && pat->pat[OFFSET(pat, x + 1, y    )] == index) x++;
        else
        {
            break;
        }
        index++;
        P_snake_newseg(edge_left + x, edge_up + y);
    }
}

/**
 * @brief Erase the snake
 */
void snake_done(void)
{
    struct snake_seg *sseg;
    while(snake.head)
    {
        sseg       = snake.head;
        snake.head = snake.head->next;
        gamelib.geng->world_ent_unlink(sseg->ient);
        Z_free(sseg);
    }
}

/**
 * @brief Get the purgen
 */
void snake_get_purgen(void)
{
    int num = 2;
    struct snake_seg *sseg;
    sseg=snake.head;
    while(sseg->next) sseg = sseg->next;
    while(num > 0 && sseg->prev)
    {
        obj_new(sseg->origin.x, sseg->origin.y, OBJ_SHIT);
        sseg = sseg->prev;
        gamelib.geng->world_ent_unlink(sseg->next->ient);
        Z_free(sseg->next);
        sseg->next = NULL;
        P_snake_seg_model_update(sseg);
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
void snake_think(void)
{
    struct snake_seg *sseg;
    struct snake_seg *pt;
    struct obj_st *obj;

    if(!ringbuf_check_empty(&snake.commands_queue))
    {
        size_t head = ringbuf_head_get(&snake.commands_queue);
        enum direction direction = snake.commands[head];
        ringbuf_dequeue_head(&snake.commands_queue);

        if(snake.head != NULL)
        {
            struct snake_seg *neck = snake.head->next;
            switch(direction)
            {
                case DIRECTION_NORTH: /* fallthrough */
                case DIRECTION_SOUTH: /* fallthrough */
                    if(neck == NULL || neck->origin.x!=snake.head->origin.x)
                    {
                        snake.movedir = direction;
                    }
                    break;
                case DIRECTION_WEST: /* fallthrough */
                case DIRECTION_EAST: /* fallthrough */
                    if(neck == NULL || neck->origin.y!=snake.head->origin.y)
                    {
                        snake.movedir = direction;
                    }
                    break;
            }
        }
    }

    snake.level = snake.scores / SCORES_PER_LEVEL;

    if(snake.level >= LEVEL_MAX)
    {
        snake.level = LEVEL_MAX - 1;
    }

    obj = P_snake_obj_check();
    if(obj != NULL)
    {
        /* objects eating */
        switch(obj->type)
        {
            case OBJ_MARIJUANA:
                snake.scores++;
                P_snake_newseg(obj->origin.x, obj->origin.y);
                obj_put(OBJ_MARIJUANA);
                if(rand()%3==1) obj_put(OBJ_PURGEN);
                break;
            case OBJ_MARIJUANAP:
                /* growed up */
                snake.scores++;
                P_snake_newseg(obj->origin.x, obj->origin.y);
                break;
            case OBJ_PURGEN:
                snake_get_purgen();
                break;
            case OBJ_SHIT:
                snake_get_shit();
                break;
        }

        obj_free(&obj);

        sseg = snake.head;
        while(sseg->next)
        {
            sseg = sseg->next;
        }
    }
    else
    {
        /* moving */
        if(snake.head->next == NULL)
        {
            struct snake_seg * sseg = snake.head;

            switch(snake.movedir)
            {
                case DIRECTION_NORTH: sseg->origin.y--;break;
                case DIRECTION_SOUTH: sseg->origin.y++;break;
                case DIRECTION_WEST : sseg->origin.x--;break;
                case DIRECTION_EAST : sseg->origin.x++;break;
            }
            gamelib.geng->world_ent_update_orig(sseg->ient, &sseg->origin);
        }
        else
        {
            /* more than one segment */
            struct snake_seg * head;
            struct snake_seg * tail;
            tail = snake.head;
            while(tail->next != NULL)
            {
                tail = tail->next;
            }
            head = tail;
            /* cut the ass... */
            tail = tail->prev;
            tail->next = NULL;
            /* ... and make it a head */
            head->prev = NULL;
            head->next = snake.head;
            snake.head->prev = head;
            snake.head       = head;
            sseg = head->next; /* previous head */
            switch(snake.movedir)
            {
                case DIRECTION_NORTH: head->origin.x = sseg->origin.x  ; head->origin.y = sseg->origin.y-1; break;
                case DIRECTION_SOUTH: head->origin.x = sseg->origin.x  ; head->origin.y = sseg->origin.y+1; break;
                case DIRECTION_WEST : head->origin.x = sseg->origin.x-1; head->origin.y = sseg->origin.y  ; break;
                case DIRECTION_EAST : head->origin.x = sseg->origin.x+1; head->origin.y = sseg->origin.y  ; break;
            }
            P_snake_seg_model_update(head);
            P_snake_seg_model_update(tail);
            gamelib.geng->world_ent_update_orig(head->ient, &head->origin);
        }
    }
    /* self-cross check */
    pt = snake.head;
    while(pt && !snake.dead)
    {
        sseg=pt->next;
        while(sseg != NULL)
        {
            if(origins_eq(&pt->origin, &sseg->origin))
            {
                snake_die();
                break;
            }
            sseg = sseg->next;
        }
        pt = pt->next;
    }
    /* проверка на выход за границу карты */
    if(snake.head->origin.x < 0 || MAP_SX <= snake.head->origin.x
            || snake.head->origin.y < 0 || MAP_SY <= snake.head->origin.y)
    {
        snake_die();
    }
}

void snake_die(void)
{
    snake.dead = true;

    struct snake_seg * sseg;
    for(sseg = snake.head; sseg; sseg = sseg->next)
    {
        P_snake_seg_model_update(sseg);
    }
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
void player_setdir(enum direction movedir)
{
    if(ringbuf_check_full(&snake.commands_queue))
    {
        return;
    }
    size_t tail = ringbuf_tail_get(&snake.commands_queue);
    snake.commands[tail] = movedir;
    ringbuf_enqueue_tail(&snake.commands_queue);
}

enum direction player_direction(void)
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

