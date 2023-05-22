#include "menu.h"
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