//25.03.2004
//
//змея
//

#include "game.h"

int main()
{
    game_init();
    while(!game_is_quit())
    {
        game_events_pump();
        game_tick();
        game_draw();
    }
    game_done();
    return 0 ;
}
