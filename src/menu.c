#include "menu.h"
#include "platform.h"
#include <stdio.h>
#include <stdlib.h>

void new_game(u32 *player_number, char player_name[MAX_PLAYER][PLAYER_NAME_LENGTH], Class_Type player_class[MAX_PLAYER]) {
    i32 correct = 0;
    printf("Loading a new game...\n");
    // ask the number of player
    printf("How much player will play? You can play up to 4 peoples.\n");
    do {
        printf(">> ");
        correct = scanf("%d", player_number);
        empty_stdin_buffer();
    } while (*player_number < 0 || *player_number > MAX_PLAYER || correct != 1);

    // ask player names
    for (u32 count = 0; count < *player_number; count++) {
        printf("Type the name of the player %d: ", count + 1);
        // TODO save all char as lower, then print as upper for the first one
        scanf("%s", player_name[count]);
        empty_stdin_buffer();
        // ask the player his class
        game_choose_class(player_class, count);
    }
}

void reset_variables(u32 *player_number, u32 treasure_found[MAX_PLAYER], u32 monster_killed[MAX_PLAYER], u32 round_number[MAX_PLAYER],
                     u32 treasure[MAX_PLAYER], b8 will_teleport[MAX_PLAYER], b8 artifact_found[MAX_PLAYER],
                     u32 player_x[MAX_PLAYER], u32 player_y[MAX_PLAYER], Choosen_Weapon active_weapon[MAX_PLAYER], Class_Type player_class[MAX_PLAYER],
                     char player_name[MAX_PLAYER][PLAYER_NAME_LENGTH], Case_Type *turn, b8 *is_winner) {
    // for counting the score
    player_number = 0;
    platform_zero_memory(treasure_found, sizeof(u32) * MAX_PLAYER);
    platform_zero_memory(monster_killed, sizeof(u32) * MAX_PLAYER);

    // for the current round
    platform_zero_memory(round_number, sizeof(u32) * MAX_PLAYER);
    platform_zero_memory(treasure, sizeof(u32) * MAX_PLAYER);
    platform_zero_memory(will_teleport, sizeof(b8) * MAX_PLAYER);
    platform_zero_memory(artifact_found, sizeof(b8) * MAX_PLAYER);
    platform_zero_memory(player_x, sizeof(u32) * MAX_PLAYER);
    platform_zero_memory(player_y, sizeof(u32) * MAX_PLAYER);
    platform_zero_memory(active_weapon, sizeof(Choosen_Weapon) * MAX_PLAYER);

    // for the current game
    // platform_set_memory(player_class, CLASS_UNKNOWN, sizeof(Class_Type) * MAX_PLAYER);
    player_class[0] = CLASS_UNKNOWN;
    player_class[1] = CLASS_UNKNOWN;
    player_class[2] = CLASS_UNKNOWN;
    player_class[3] = CLASS_UNKNOWN;
    *turn = PLAYER_BLUE;
    *is_winner = false;

    // setup the coordinates for the start
    // player blue
    player_x[0] = 0;
    player_y[0] = 2;
    // player green
    player_x[1] = 4;
    player_y[1] = 0;
    // player white
    player_x[2] = MAP_SIZE - 1;
    player_y[2] = 4;
    // player yellow
    player_x[3] = 2;
    player_y[3] = MAP_SIZE - 1;
}