/**
 * 25.03.2004
 *
 * snake
 *
 */

/*
defines:
    _DEFAULT_SOURCE
libs:
    ncursesw
 */

#include "io.h"
#include "game.h"

int main()
{
    io_init();
    game_init();
    while(!game_is_quit())
    {
        game_loop();
        game_draw();
    }
    game_done();
    io_done();
    return 0;
}
