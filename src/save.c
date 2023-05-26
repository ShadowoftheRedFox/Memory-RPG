#include "./save.h"
#include "./game.h"
#include "./menu.h"
#include "./platform.h"
#include <string.h>

b8 save_file_exists(const char *path) {
    // verify parameters
    if (path == NULL) {
        printf("path is null in save_file_exists\n");
        exit(1);
    }

    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return false;
    }
    fclose(f);
    return true;
}

/*
TODO save multiple game instances:
unique number for new game save -> when saving, remember that number in a variable
-> unique number with time(); see how to get current time
when deleting save, delete while searching the name -> add a delete save in menu options
when loading, ask for which save to load, by showing which player there are in each game and how many round has been played,
and when he has been created
^If there is o bug after the test on linux
!It's a pain to look what files there is into a folder, so save the number in a number.save or something
*/

void get_save_name(char *path, u32 save_id) {
    sprintf(path, SAVE_FOLDER "memory%06d.save", save_id);
}

// delete the save file after the party has ended
b8 remove_save(u32 save_id) {
    char temp[100];
    get_save_name(temp, save_id);
    return remove(temp);
}

b8 save_game(Board_Case **map, u32 player_number, u32 treasure_found[MAX_PLAYER], u32 monster_killed[MAX_PLAYER], u32 round_number[MAX_PLAYER],
             u32 treasure[MAX_PLAYER], b8 will_teleport[MAX_PLAYER], b8 artifact_found[MAX_PLAYER],
             u32 player_x[MAX_PLAYER], u32 player_y[MAX_PLAYER], Choosen_Weapon active_weapon, Class_Type player_class[MAX_PLAYER],
             char player_name[MAX_PLAYER][PLAYER_NAME_LENGTH], Case_Type turn, b8 is_winner, u32 save_id) {
    // verify parameters
    if (map == NULL) {
        printf("Map is null on save_game\n");
        exit(2);
    }
    if (player_number < 0 || player_number > MAX_PLAYER) {
        printf("player_number is out of range in save_game\n");
        exit(1);
    }
    if (treasure_found == NULL) {
        printf("treasure_found is null in save_game\n");
        exit(1);
    }
    if (monster_killed == NULL) {
        printf("monster_killed is null in save_game\n");
        exit(1);
    }
    if (round_number == NULL) {
        printf("round_number is null in save_game\n");
        exit(1);
    }
    if (treasure == NULL) {
        printf("treasure is null in save_game\n");
        exit(1);
    }
    if (will_teleport == NULL) {
        printf("will_teleport is null in save_game\n");
        exit(1);
    }
    if (artifact_found == NULL) {
        printf("artifact_found is null in save_game\n");
        exit(1);
    }
    if (player_x == NULL) {
        printf("player_x is null in save_game\n");
        exit(1);
    }
    if (player_y == NULL) {
        printf("player_y is null in save_game\n");
        exit(1);
    }
    if (active_weapon < 0 || active_weapon >= WEAPON_UNKNOWN) {
        printf("active_weapon is out of range in save_game\n");
        exit(1);
    }
    if (player_class == NULL) {
        printf("player_class is null in save_game\n");
        exit(1);
    }
    if (player_name == NULL) {
        printf("player_name is null in save_game\n");
        exit(1);
    }
    if (turn < PLAYER_BLUE || turn >= TYPE_UNKNOWN) {
        printf("turn is out of range in save_game\n");
        exit(1);
    }
    if (is_winner != true && is_winner != false) {
        printf("is_winner is out of range in save_game\n");
        exit(1);
    }
    if (save_id <= 0) {
        printf("save_id is out of range in save_game\n");
        exit(1);
    }

    //  create the file handle
    char save_name[100];
    get_save_name(save_name, save_id);
    FILE *save_file = fopen(save_name, "wb");
    if (save_file == NULL) {
        printf("Failed to get the save file in save_game\n");
        // failed to save
        return false;
    }

    // save the save_id
    if (!save_file_id(save_id, player_name, player_number)) {
        // failed to save the save id
        printf("Failed to save the save id in save_game\n");
        fclose(save_file);
        remove_save(save_id);
        return false;
    }

    // fill in the struct that we'll save
    Save_File_Struct file_struct;
    file_struct.active_weapon = active_weapon;
    file_struct.turn = turn;
    file_struct.is_winner = is_winner;
    file_struct.player_number = player_number;

    // save for all case through a loop
    for (u8 y = 0; y < MAP_SIZE; y++) {
        for (u8 x = 0; x < MAP_SIZE; x++) {
            file_struct.map[y][x] = map[y][x];
        }
    }

    for (u8 i = 0; i < MAX_PLAYER; i++) {
        strcpy(file_struct.player_name[i], player_name[i]);
        file_struct.treasure_found[i] = treasure_found[i];
        file_struct.monster_killed[i] = monster_killed[i];
        file_struct.round_number[i] = round_number[i];
        file_struct.treasure[i] = treasure[i];
        file_struct.will_teleport[i] = will_teleport[i];
        file_struct.artifact_found[i] = artifact_found[i];
        file_struct.player_x[i] = player_x[i];
        file_struct.player_y[i] = player_y[i];
        file_struct.player_class[i] = player_class[i];
    }

    // write in the file
    fwrite(&file_struct, sizeof(Save_File_Struct), 1, save_file);

    // close stream
    fclose(save_file);

    // save success
    return true;
}

b8 load_game(Board_Case **map, u32 *player_number, u32 treasure_found[MAX_PLAYER], u32 monster_killed[MAX_PLAYER], u32 round_number[MAX_PLAYER],
             u32 treasure[MAX_PLAYER], b8 will_teleport[MAX_PLAYER], b8 artifact_found[MAX_PLAYER],
             u32 player_x[MAX_PLAYER], u32 player_y[MAX_PLAYER], Choosen_Weapon *active_weapon, Class_Type player_class[MAX_PLAYER],
             char player_name[MAX_PLAYER][PLAYER_NAME_LENGTH], Case_Type *turn, b8 *is_winner, u32 save_id) {
    // verify parameters
    // we are gonna edit all those value, we just need to check if thet are not null
    if (map == NULL) {
        printf("Map is null on load_game\n");
        exit(2);
    }
    if (player_number == NULL) {
        printf("player_number is out of range in load_game\n");
        exit(1);
    }
    if (treasure_found == NULL) {
        printf("treasure_found is null in load_game\n");
        exit(1);
    }
    if (monster_killed == NULL) {
        printf("monster_killed is null in load_game\n");
        exit(1);
    }
    if (round_number == NULL) {
        printf("round_number is null in load_game\n");
        exit(1);
    }
    if (treasure == NULL) {
        printf("treasure is null in load_game\n");
        exit(1);
    }
    if (will_teleport == NULL) {
        printf("will_teleport is null in load_game\n");
        exit(1);
    }
    if (artifact_found == NULL) {
        printf("artifact_found is null in load_game\n");
        exit(1);
    }
    if (player_x == NULL) {
        printf("player_x is null in load_game\n");
        exit(1);
    }
    if (player_y == NULL) {
        printf("player_y is null in load_game\n");
        exit(1);
    }
    if (active_weapon == NULL) {
        printf("active_weapon is out of range in load_game\n");
        exit(1);
    }
    if (player_class == NULL) {
        printf("player_class is null in load_game\n");
        exit(1);
    }
    if (player_name == NULL) {
        printf("player_name is null in load_game\n");
        exit(1);
    }
    if (turn == NULL) {
        printf("turn is out of range in load_game\n");
        exit(1);
    }
    if (is_winner == NULL) {
        printf("is_winner is out of range in load_game\n");
        exit(1);
    }
    if (save_id <= 0) {
        printf("save_id is out of range in load_game\n");
        exit(1);
    }

    //  create the file handle
    char save_name[100];
    get_save_name(save_name, save_id);
    FILE *save_file = fopen(save_name, "rb");
    if (save_file == NULL) {
        printf("Failed to get the save file in load_game.\n");
        // failed to load
        return false;
    }

    // loading 1 struct from the file
    Save_File_Struct file_struct;
    if (fread(&file_struct, sizeof(Save_File_Struct), 1, save_file) != 1) {
        printf("Something went wrong when reading the data in load_game\n");
        exit(1);
    }

    // fill in the struct that we'll load
    *active_weapon = file_struct.active_weapon;
    *turn = file_struct.turn;
    *is_winner = file_struct.is_winner;
    *player_number = file_struct.player_number;

    // save for all case through a loop
    for (u8 y = 0; y < MAP_SIZE; y++) {
        for (u8 x = 0; x < MAP_SIZE; x++) {
            map[y][x] = file_struct.map[y][x];
        }
    }

    for (u8 i = 0; i < MAX_PLAYER; i++) {
        strcpy(player_name[i], file_struct.player_name[i]);
        treasure_found[i] = file_struct.treasure_found[i];
        monster_killed[i] = file_struct.monster_killed[i];
        round_number[i] = file_struct.round_number[i];
        treasure[i] = file_struct.treasure[i];
        will_teleport[i] = file_struct.will_teleport[i];
        artifact_found[i] = file_struct.artifact_found[i];
        player_x[i] = file_struct.player_x[i];
        player_y[i] = file_struct.player_y[i];
        player_class[i] = file_struct.player_class[i];
    }

    return true;
}

b8 save_score(char player_name[PLAYER_NAME_LENGTH], u32 treasure_found, u32 monster_killed, u32 game_won) {
    // verify parameters
    if (player_name == NULL) {
        printf("player_name is null in save_score\n");
        exit(1);
    }
    if (monster_killed < 0) {
        printf("monster_killed is out of range in save_score\n");
        exit(1);
    }
    if (treasure_found < 0) {
        printf("treasure_found is out of range in save_score\n");
        exit(1);
    }
    if (game_won < 0) {
        printf("game_won is out of range in save_score\n");
        exit(1);
    }

    Save_Player_Score file_struct;
    Save_Player_Score temp;
    u32 count = 0;

    FILE *file = fopen(SAVE_FOLDER SCORE_FILE_NAME, "rb+");
    if (file == NULL) {
        file = fopen(SAVE_FOLDER SCORE_FILE_NAME, "wb+");
    }

    if (file == NULL) {
        printf("Failed to get the save file in save_score.\n");
        return false;
    }

    // fill our struct to save
    strcpy(file_struct.player_name, player_name);
    file_struct.treasure_found = treasure_found;
    file_struct.monster_killed = monster_killed;
    file_struct.game_won = game_won;

    // move the cursor as we read
    // while == 1 because we want 1 item successfully read
    while (fread(&temp, sizeof(Save_Player_Score), 1, file) == 1) {
        // check if the name is the same
        if (strcmp(temp.player_name, file_struct.player_name) == 0) {
            // update the found struct
            file_struct.treasure_found += temp.treasure_found;
            file_struct.monster_killed += temp.monster_killed;
            file_struct.game_won += temp.game_won;
            break;
        }
        // follow the structure position
        count++;
    }

    // write the new data at the position of the wanted structure
    fseek(file, (long)(sizeof(Save_Player_Score)) * count, SEEK_SET);
    // check if successful
    if (fwrite(&file_struct, sizeof(Save_Player_Score), 1, file) != 1) {
        printf("failed to update data, %s\n", player_name);
        fclose(file);
        return false;
    }

    // close the stream
    fclose(file);
    return true;
}

b8 save_file_id(u32 save_id, char player_name[MAX_PLAYER][PLAYER_NAME_LENGTH], u32 player_number) {
    if (save_id <= 0) {
        printf("save_id is out of range in save_file_id\n");
        exit(1);
    }
    if (player_name == NULL) {
        printf("player_name is null in save_file_id\n");
        exit(1);
    }
    if (player_number <= 0) {
        printf("player_number is out of range in save_file_id\n");
        exit(1);
    }

    Save_Number file_struct;
    Save_Number temp;
    u32 count = 0;

    //  create the file handle
    FILE *file = fopen(SAVE_FOLDER NUMBER_FILE_NAME, "rb+");
    if (file == NULL) {
        file = fopen(SAVE_FOLDER NUMBER_FILE_NAME, "wb+");
    }
    if (file == NULL) {
        printf("Failed to get the save file in save_file_id\n");
        // failed to save
        return false;
    }

    // fill our struct
    file_struct.save_id = save_id;
    file_struct.player_number = player_number;
    file_struct.date = time(NULL);
    // to know if the associated game save has been deleted or not
    file_struct.deleted = false;
    for (u8 i = 0; i < MAX_PLAYER; i++) {
        strcpy(file_struct.player_name[i], player_name[i]);
    }

    // move the cursor as we read
    // while == 1 because we want 1 item successfully read
    while (fread(&temp, sizeof(Save_Number), 1, file) == 1) {
        // check if the name is the same
        if (temp.save_id == save_id) {
            // we already saved the file, quit
            fclose(file);
            return true;
        }
        // follow the structure position
        count++;
    }

    // go back at the beginning of the file
    fseek(file, 0, SEEK_SET);
    count = 0;
    // replace any deleted struct by our new
    while (fread(&temp, sizeof(Save_Number), 1, file) == 1) {
        // check if the name is the same
        if (temp.deleted == true) {
            // write the new data at the position of the wanted structure
            fseek(file, (long)(sizeof(Save_Number)) * count, SEEK_SET);
            // check if successful
            if (fwrite(&file_struct, sizeof(Save_Number), 1, file) != 1) {
                printf("failed to update data, %06d\n", save_id);
                fclose(file);
                return false;
            }
            fclose(file);
            return true;
        }
        // follow the structure position
        count++;
    }

    // write the new data at the position of the wanted structure
    fseek(file, (long)(sizeof(Save_Number)) * count, SEEK_SET);
    // check if successful
    if (fwrite(&file_struct, sizeof(Save_Number), 1, file) != 1) {
        printf("failed to update data, %06d\n", save_id);
        fclose(file);
        return false;
    }

    // close the stream
    fclose(file);
    return true;
}

u32 add_alloc_id(Save_Number **array, u32 index_target, u32 current_size, Save_Number *new_element) {
    // verify parameters
    if (array == NULL) {
        printf("array is null in add_alloc_id\n");
        exit(1);
    }
    if (new_element == NULL) {
        printf("new_element is null in add_alloc_id\n");
        exit(1);
    }
    if (index_target < 0) {
        printf("index_target is out of range in add_alloc_id\n");
        exit(1);
    }
    if (current_size < 0) {
        printf("current_size is out of range in add_alloc_id\n");
        exit(1);
    }

    if (index_target >= current_size) {
        // we want to write in array[index_target], so it size must be at least current_size + 1
        current_size = index_target + 1;
        // re alloc memory
        Save_Number *new_array = realloc(*array, current_size * sizeof(Save_Number));

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

void get_save_id_array(Save_Number **dest_array, u32 *dest_size) {
    // verify parameters
    if (dest_array == NULL) {
        printf("dest_array is out of range in get_save_id_save\n");
        exit(1);
    }
    if (dest_size == NULL) {
        printf("dest_size is out of range in get_save_id_save\n");
        exit(1);
    }

    // size of our array
    u32 array_size = 0;
    // number of save struct found
    u32 struct_found = 0;
    // our array
    Save_Number *array = platform_allocate(sizeof(Save_Number) * array_size);
    // temporrary element, we'll stock information we read in it
    Save_Number temp;

    FILE *file = fopen(SAVE_FOLDER NUMBER_FILE_NAME, "rb");
    if (file == NULL) {
        printf("Failed to get the save file in get_save_id_save.\n");
        exit(1);
    }

    // get each struct and save it in the array
    while (fread(&temp, sizeof(temp), 1, file) == 1) {
        array_size = add_alloc_id(&array, struct_found, array_size, &temp);
        struct_found++;
    }

    // we replace the array provided and it's length by ours
    *dest_array = array;
    *dest_size = array_size;
}

void remove_save_id(u32 save_id) {
    if (save_id <= 0) {
        printf("save_id is out of range in remove_save_id\n");
        exit(1);
    }

    Save_Number temp;
    u32 count = 0;

    //  create the file handle
    FILE *file = fopen(SAVE_FOLDER NUMBER_FILE_NAME, "rb+");
    if (file == NULL) {
        printf("Failed to get the save file in remove_save_id\n");
        // failed to edit
        return;
    }

    // move the cursor as we read
    // while == 1 because we want 1 item successfully read
    while (fread(&temp, sizeof(Save_Number), 1, file) == 1) {
        // check if the name is the same
        if (temp.save_id == save_id) {
            // we found our struct, set the flag and quit
            temp.deleted = true;
            fseek(file, (long)(sizeof(Save_Number)) * count, SEEK_SET);
            // check if successful
            if (fwrite(&temp, sizeof(Save_Number), 1, file) != 1) {
                printf("failed to set flag data, %06d\n", save_id);
            }
            fclose(file);
            return;
        }
        // follow the structure position
        count++;
    }

    printf("failed to find data, %06d\n", save_id);

    // close the stream
    fclose(file);
}