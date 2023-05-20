#include "./game.h"
#include "./platform.h"
#include "./save.h"

int main(int argc, char const *argv[]) {
    // setup game
    platform_srand();

    // create the map
    Board_Case **map = map_create();

    // setup variables
    b8 game_running = true;
    u8 confirm = 0;
    i8 correct = 0;
    u8 turn_number = 0;

    // for counting the score
    u32 player_number = 0;
    u32 treasure_found[MAX_PLAYER] = {0};
    u32 monster_killed[MAX_PLAYER] = {0};

    // for the current round
    u32 round_number[MAX_PLAYER] = {0};
    u32 treasure[MAX_PLAYER] = {0};
    b8 will_teleport[MAX_PLAYER] = {false};
    b8 artifact_found[MAX_PLAYER] = {false};
    u32 player_x[MAX_PLAYER] = {0};
    u32 player_y[MAX_PLAYER] = {0};
    Choosen_Weapon active_weapon[MAX_PLAYER] = {0};

    // for the current game
    Class_Type player_class[MAX_PLAYER];
    char player_name[MAX_PLAYER][PLAYER_NAME_LENGTH];
    Case_Type turn = PLAYER_BLUE;
    b8 is_winner = false;

    // ask the number of player
    printf("How much player will play? You can play up to 4 peoples.\n");
    do {
        printf(">> ");
        correct = scanf("%d", &player_number);
        empty_stdin_buffer();
    } while (player_number < 0 || player_number > MAX_PLAYER || correct != 1);

    // ask player names
    for (u32 count = 0; count < player_number; count++) {
        printf("Type the name of the player %d: ", count + 1);
        scanf("%s", player_name[count]);
        empty_stdin_buffer();
        // ask the player his class
        game_choose_class(&(player_class[count]));
    }

    // TODO make sure player can't get the same class

    // setup the coordinates for the first player (blue)
    player_y[0] = 2;
    player_x[0] = 0;

    // TODO load and save player score and name
    // TODO best score leaderboard?
    // TODO main menu before game with: play, resume game, score, quit
    // TODO round system

    while (game_running) {
        // [turn - PLAYER_BLUE] gives the correct number in the array depending of the turn, whatever is the number of player
        turn_number = turn - PLAYER_BLUE;

        // say whose turn is who
        printf("It's %s turn!\n\n", player_name[turn_number]);
        //  show the board
        map_print(map);

        // ask for which weapon for this round
        game_choose_weapon(&active_weapon[turn_number]);

        // await player move
        if (!will_teleport[turn_number]) {
            player_move(map, &turn, player_number, &player_x[turn_number], &player_y[turn_number]);
        } else {
            will_teleport[turn_number] = false;
            player_teleport(map, &turn, &player_x[turn_number], &player_y[turn_number]);
        }

        // logic after the move
        game_logic(map, &turn, &treasure[turn_number], &monster_killed[turn_number], &artifact_found[turn_number],
                   player_class[turn_number], active_weapon[turn_number], player_number,
                   &will_teleport[turn_number], &player_x[turn_number], &player_y[turn_number], &is_winner);

        if (is_winner) {
            game_win(turn, player_name[turn_number], round_number[turn_number], &game_running);
        }
    }

    map_destroy(map);
    return 0;
}
// BUG for 2 players, 2nd player can't move anywhere
//  didn't test for more