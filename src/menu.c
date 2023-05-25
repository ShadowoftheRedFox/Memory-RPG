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
    } while (*player_number <= 0 || *player_number > MAX_PLAYER || correct != 1);

    // ask player names
    for (u32 count = 0; count < *player_number; count++) {
        printf("Type the name of the player %d: ", count + 1);
        scanf("%s", player_name[count]);
        empty_stdin_buffer();
        // ask the player his class
        game_choose_class(player_class, count);
    }
}

void reset_variables(u32 *player_number, u32 treasure_found[MAX_PLAYER], u32 monster_killed[MAX_PLAYER], u32 round_number[MAX_PLAYER],
                     u32 treasure[MAX_PLAYER], b8 will_teleport[MAX_PLAYER], b8 artifact_found[MAX_PLAYER],
                     u32 player_x[MAX_PLAYER], u32 player_y[MAX_PLAYER], Choosen_Weapon *active_weapon, Class_Type player_class[MAX_PLAYER],
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
    *active_weapon = WEAPON_UNKNOWN;

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

void score_menu() {
    // exit if no score file found
    if (!save_file_exists(SAVE_FOLDER SCORE_FILE_NAME)) {
        printf("No on player the game yet. There is no score to display.\n");
        return;
    }

    b8 score_running = true;
    u32 menu = 0;
    i32 correct = 0;

    // menu for the score
    while (score_running) {
        // choice for each score types
        printf("SCORE\n1 - TOP PLAYERS\n2 - TOP KILLERS\n3 - TOP TREASURES HUNTERS\n4 - TOP WINNERS\n5 - BACK TO MAIN MENU\n");
        do {
            printf(">> ");
            correct = scanf("%d", &menu);
        } while (menu < 1 || menu > 5 || correct != 1);

        // display wanted menu
        switch (menu) {
        case 1:
            printf("Those are the top players with the best scores:\n\n");
            show_score(SORT_TOP);
            break;
        case 2:
            printf("Those are the top players with the biggest amount of kill:\n\n");
            show_score(SORT_KILL);
            break;
        case 3:
            printf("Those are the top players with the biggest amount of treasure found:\n\n");
            show_score(SORT_TREASURE);
            break;
        case 4:
            printf("Those are the top players with the biggest amount of win:\n\n");
            show_score(SORT_WIN);
            break;
        case 5:
            score_running = false;
            break;
        default:
            printf("unhandled value in score_menu: %d\n", menu);
            break;
        }
    }
}

b8 add_alloc(Save_Player_Score **array, u32 index_target, u32 current_size, Save_Player_Score *new_element) {
    if (index_target >= current_size) {
        // we want to write in array[index_target], so it size must be at least current_size + 1
        current_size = index_target + 1;
        // re alloc memory
        Save_Player_Score *new_array = realloc(*array, current_size * sizeof(Save_Player_Score));

        // failed to re alloc memory
        if (new_array == NULL) {
            return false;
        }

        // exchange new address value with the old one
        *array = new_array;
    }

    // update the memory
    (*array)[index_target] = *new_element;
    // return the new size
    return current_size;
}

void show_score(Sort_Type type) {
    // size of our array
    u32 array_size = 0;
    // number of save struct found
    u32 struct_found = 0;
    // our array
    Save_Player_Score *array = platform_allocate(sizeof(Save_Player_Score) * array_size);
    // temporrary element, we'll stock information we read in it
    Save_Player_Score temp;

    FILE *file = fopen(SAVE_FOLDER SCORE_FILE_NAME, "rb");
    if (file == NULL) {
        printf("Failed to get the save file in show_score.\n");
    }

    // get each struct and save it in the array
    while (fread(&temp, sizeof(temp), 1, file) == 1) {
        array_size = add_alloc(&array, struct_found, array_size, &temp);
        struct_found++;
    }

    // sort the values
    quickSort(array, 0, array_size - 1, type);

    // display the sorted array
    switch (type) {
    case SORT_KILL:
        for (u32 i = 0; i < struct_found; i++) {
            printf("%d: %s:\t%d\n", i + 1, array[i].player_name, array[i].monster_killed);
        }
        break;
    case SORT_WIN:
        for (u32 i = 0; i < struct_found; i++) {
            printf("%d: %s:\t%d\n", i + 1, array[i].player_name, array[i].game_won);
        }
        break;
    case SORT_TREASURE:
        for (u32 i = 0; i < struct_found; i++) {
            printf("%d: %s:\t%d\n", i + 1, array[i].player_name, array[i].treasure_found);
        }
        break;
    case SORT_TOP:
        for (u32 i = 0; i < struct_found; i++) {
            printf("%d: %s:\t%d\n", i + 1, array[i].player_name,
                   array[i].game_won * SCORE_WIN + array[i].monster_killed * SCORE_KILL + array[i].treasure_found * SCORE_TREASURE);
        }
        break;
    default:
        printf("type is our of range in show_score: %d\n", type);
        break;
    }

    // free the resources used
    platform_free(array);
    // close the stream
    fclose(file);
}