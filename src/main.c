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
#include "render.h"
#include <stdlib.h>

int main()
{
    if(game_init())
    {
        return EXIT_FAILURE;
    }
    while(!game_is_quit())
    {
        render_begin();

        game_loop();
        game_render();

        render_end();
    }
    game_done();
    return EXIT_SUCCESS;
}
