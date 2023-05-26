#include "menu.h"
#include "platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void new_game(u32 *player_number, char player_name[MAX_PLAYER][PLAYER_NAME_LENGTH], Class_Type player_class[MAX_PLAYER]) {
    // verify parameters
    if (player_number == NULL) {
        // we don't check for out of range because we'll edit it here
        printf("player_number is null in new_game\n");
        exit(1);
    }
    if (player_name == NULL) {
        printf("player_name is null in new_game\n");
        exit(1);
    }
    if (player_class == NULL) {
        printf("player_class is null in new_game\n");
        exit(1);
    }

    i32 correct = 0;
    animate_printf("Loading a new game...\n");
    // ask the number of player
    animate_printf("How much player will play? You can play up to 4 peoples.\n");
    do {
        printf(">> ");
        correct = scanf("%d", player_number);
        empty_stdin_buffer();
    } while (*player_number <= 0 || *player_number > MAX_PLAYER || correct != 1);

    // ask player names
    for (u32 count = 0; count < *player_number; count++) {
        char temp[40];
        sprintf(temp, "Type the name of the player %d: ", count + 1);
        animate_printf(temp);

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
    // verify parameters, we don't check for out of range since we'll edit them here
    if (player_number == NULL) {
        printf("player_number is null in reset_varaiables\n");
        exit(1);
    }
    if (treasure_found == NULL) {
        printf("treasure_found is null in reset_varaiables\n");
        exit(1);
    }
    if (monster_killed == NULL) {
        printf("monster_killed is null in reset_varaiables\n");
        exit(1);
    }
    if (round_number == NULL) {
        printf("round_number is null in reset_varaiables\n");
        exit(1);
    }
    if (treasure == NULL) {
        printf("treasure is null in reset_varaiables\n");
        exit(1);
    }
    if (will_teleport == NULL) {
        printf("will_teleport is null in reset_varaiables\n");
        exit(1);
    }
    if (artifact_found == NULL) {
        printf("artifact_found is null in reset_varaiables\n");
        exit(1);
    }
    if (player_x == NULL) {
        printf("player_x is null in reset_varaiables\n");
        exit(1);
    }
    if (player_y == NULL) {
        printf("player_y is null in reset_varaiables\n");
        exit(1);
    }
    if (active_weapon == NULL) {
        printf("active_weapon is null in reset_varaiables\n");
        exit(1);
    }
    if (player_class == NULL) {
        printf("player_class is null in reset_varaiables\n");
        exit(1);
    }
    if (player_name == NULL) {
        printf("player_name is null in reset_varaiables\n");
        exit(1);
    }
    if (turn == NULL) {
        printf("turn is null in reset_varaiables\n");
        exit(1);
    }
    if (is_winner == NULL) {
        printf("is_winner is null in reset_varaiables\n");
        exit(1);
    }

    // for counting the score
    *player_number = 0;
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
        animate_printf("No one played the game yet. There is no score to display.\n");
        platform_sleep(750);
        return;
    }

    b8 score_running = true;
    u32 menu = 0;
    i32 correct = 0;

    // menu for the score
    while (score_running) {
        // choice for each score types
        platform_console_clear();
        animate_printf("Score:\n1 - Top players\n2 - Top killers\n3 - Top treasures hunters\n4 - Top winners\n5 - Back to main menu\n");
        do {
            printf(">> ");
            correct = scanf("%d", &menu);
            empty_stdin_buffer();
        } while (menu < 1 || menu > 5 || correct != 1);

        // display wanted menu
        switch (menu) {
        case 1:
            animate_printf("Those are the top players with the best scores:\n\n");
            show_score(SORT_TOP);
            break;
        case 2:
            animate_printf("Those are the top players with the biggest amount of kill:\n\n");
            show_score(SORT_KILL);
            break;
        case 3:
            animate_printf("Those are the top players with the biggest amount of treasure found:\n\n");
            show_score(SORT_TREASURE);
            break;
        case 4:
            animate_printf("Those are the top players with the biggest amount of win:\n\n");
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

u32 add_alloc(Save_Player_Score **array, u32 index_target, u32 current_size, Save_Player_Score *new_element) {
    // verify parameters
    if (array == NULL) {
        printf("array is null in add_alloc\n");
        exit(1);
    }
    if (new_element == NULL) {
        printf("new_element is null in add_alloc\n");
        exit(1);
    }
    if (index_target < 0) {
        printf("index_target is out of range in add_alloc\n");
        exit(1);
    }
    if (current_size < 0) {
        printf("current_size is out of range in add_alloc\n");
        exit(1);
    }

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
    // verify parameters
    if (type < 0 || type >= SORT_UNKNOWN) {
        printf("type is out of range in show_score\n");
        exit(1);
    }

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
        exit(1);
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
        confirm();
        break;
    case SORT_WIN:
        for (u32 i = 0; i < struct_found; i++) {
            printf("%d: %s:\t%d\n", i + 1, array[i].player_name, array[i].game_won);
        }
        confirm();
        break;
    case SORT_TREASURE:
        for (u32 i = 0; i < struct_found; i++) {
            printf("%d: %s:\t%d\n", i + 1, array[i].player_name, array[i].treasure_found);
        }
        confirm();
        break;
    case SORT_TOP:
        for (u32 i = 0; i < struct_found; i++) {
            printf("%d: %s:\t%d\n", i + 1, array[i].player_name,
                   array[i].game_won * SCORE_WIN + array[i].monster_killed * SCORE_KILL + array[i].treasure_found * SCORE_TREASURE);
        }
        confirm();
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

void confirm() {
    u8 toto;
    i32 correct = 0;
    animate_printf("\nPress any key to continue...");
    correct = scanf("%c", &toto);
    if (correct != 1) {
        empty_stdin_buffer();
    }
    platform_console_clear();
}

void animate_printf(const char *text) {
    // verify parameters
    if (text == NULL) {
        printf("text is null in animate_printf\n");
        exit(1);
    }
    u32 i = 0;
    while (text[i] != '\0') {
        printf("%c", text[i]);
        // force fflush to display
        fflush(stdout);
        platform_sleep(ANIMATION_SPEED);
        i++;
    }
}

void load_menu(u32 *answer) {
    // exit if no number file found
    if (!save_file_exists(SAVE_FOLDER NUMBER_FILE_NAME)) {
        animate_printf("There is no game in progress, so there is no game to load.\n");
        platform_sleep(2000);
        return;
    }

    platform_console_clear();

    i32 correct = 0;
    u32 choosen = 0;

    // get our current available save
    Save_Number *array = NULL;
    u32 array_size = 0;
    get_save_id_array(&array, &array_size);

    // verify returns of this function
    if (array == NULL) {
        printf("get_save_id_array returned null array\n");
        exit(1);
    }
    if (array_size <= 0) {
        printf("get_save_id_array returned out of range array_size\n");
        exit(1);
    }

    // pretty print all available save to load
    printf("There are currently %d games that are not finished:\n", array_size);
    for (u32 i = 0; i < array_size; i++) {
        printf("%d: Game with ", i + 1);
        // display the name of each player in this save
        for (u8 p = 0; p < array[i].player_number; p++) {
            animate_printf(array[i].player_name[p]);
            animate_printf(", ");
        }
        printf("started the ");
        // display the date
        print_time(array[i].save_id);
    }

    printf("\nWhich one do you want to load? Press 0 to go back to the main menu.\n");

    do {
        printf(">> ");
        correct = scanf("%d", &choosen);
        empty_stdin_buffer();
    } while (choosen < 0 || choosen > array_size || correct != 1);

    // go back to main menu
    if (choosen == 0) {
        return;
    }

    // passing by a variable because array is dynamicly allocated, so we need to free it
    *answer = array[choosen - 1].save_id;
    platform_free(array);
}

void print_time(u32 time_second) {
    if (time_second < 0) {
        printf("time_second is out of range in print_time\n");
        exit(1);
    }

    // get the time struct from time.h with our given amount of seconds
    struct tm tm = *gmtime((time_t *)(&time_second));
    // display the date
    char temp[40];
    sprintf(temp, "%02d-%02d-%4d at %02d:%02d:%02d\n",
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
            tm.tm_hour + UTC_OFFSET, tm.tm_min, tm.tm_sec);
    animate_printf(temp);
}