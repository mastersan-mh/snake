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
    if(game_init())
    {
        return EXIT_FAILURE;
    }
    io_init();
    while(!game_is_quit())
    {
        game_loop();
        game_draw();
    }
    io_done();
    game_done();
    return EXIT_SUCCESS;
}
