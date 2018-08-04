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
#include <stdlib.h>

int main()
{
    io_init();
    if(game_init())
    {
        return EXIT_FAILURE;
    }
    while(!game_is_quit())
    {
        game_loop();
        game_draw();
    }
    game_done();
    io_done();
    return EXIT_SUCCESS;
}
