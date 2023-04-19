#include "./game.h"
#include "./platform.h"
#include "./save.h"

int main(int argc, char const *argv[]) {
    // setup game
    platform_srand();

    // setup variables
    b8 game_running = true;
    u8 confirm;

    // create game variables
    Case_Type turn = PLAYER_BLUE;
    Board_Case **map = map_create();

    while (game_running) {
        // show the board

        map_print(map);

        scanf("%c", &confirm);
        if (confirm != '\n') {
            game_running = false;
        }
    }
    return 0;
}
