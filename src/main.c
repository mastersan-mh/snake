/**
 * 2018
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

#include "game.h"
#include "render.h"

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
