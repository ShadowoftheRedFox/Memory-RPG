#include "./game.h"
#include "./platform.h"
#include "./save.h"

int main(int argc, char const *argv[]) {
    // setup game
    platform_srand();

    // setup variables
    b8 game_running = true;
    u8 confirm;
    Case_Type turn = PLAYER_BLUE;

    while (game_running) {
        // show the board

        scanf("%c", &confirm);
        if (confirm != '\n') {
            game_running = false;
        }
    }
    return 0;
}
