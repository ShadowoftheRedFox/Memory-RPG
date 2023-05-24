#include "./save.h"
#include "./game.h"
#include "./platform.h"
#include <string.h>

b8 save_file_exists() {
    FILE *f = fopen(SAVE_FOLDER SAVE_FILE_NAME, "r");
    if (f == NULL) {
        return false;
    }
    fclose(f);
    return true;
}

// delete the save file after the party has ended
b8 remove_save() {
    return remove(SAVE_FOLDER SAVE_FILE_NAME);
}

b8 save_game(Board_Case **map, u32 player_number, u32 treasure_found[MAX_PLAYER], u32 monster_killed[MAX_PLAYER], u32 round_number[MAX_PLAYER],
             u32 treasure[MAX_PLAYER], b8 will_teleport[MAX_PLAYER], b8 artifact_found[MAX_PLAYER],
             u32 player_x[MAX_PLAYER], u32 player_y[MAX_PLAYER], Choosen_Weapon active_weapon, Class_Type player_class[MAX_PLAYER],
             char player_name[MAX_PLAYER][PLAYER_NAME_LENGTH], Case_Type turn, b8 is_winner) {
    // verify parameters
    if (map == NULL) {
        printf("Map is null on save_game\n");
        exit(2);
    }
    // TODO check other parameters

    //  create the file handle
    FILE *save_file = fopen(SAVE_FOLDER SAVE_FILE_NAME, "wb");
    if (save_file == NULL) {
        printf("Failed to get the save file in save_game\n");
        // failed to save
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
             char player_name[MAX_PLAYER][PLAYER_NAME_LENGTH], Case_Type *turn, b8 *is_winner) {
    // verify parameters
    if (map == NULL) {
        printf("Map is null on load_game\n");
        exit(2);
    }
    if (turn == NULL) {
        printf("turn is null on load_game\n");
        exit(2);
    }
    // TODO check other parameters
    //  create the file handle
    FILE *save_file = fopen(SAVE_FOLDER SAVE_FILE_NAME, "rb");
    if (save_file == NULL) {
        printf("Failed to get the save file in load_game.\n");
        // failed to load
        return false;
    }

    // loading 1 struct from the file
    Save_File_Struct file_struct;
    fread(&file_struct, sizeof(Save_File_Struct), 1, save_file);

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
    Save_Player_Score file_struct;
    Save_Player_Score temp;
    u64 file_size;
    u64 struct_amount;
    // TODO check other parameters

    FILE *file = fopen(SAVE_FOLDER SCORE_FILE_NAME, "ab+");
    if (file == NULL) {
        printf("Failed to get the save file in save_score.\n");
        return false;
    }

    // fill our struct to save
    strcpy(file_struct.player_name, player_name);
    file_struct.treasure_found = treasure_found;
    file_struct.game_won = game_won;
    file_struct.monster_killed = monster_killed;

    // get the file size
    // move to the end of the file
    // since we are openning the file in append mode, the cursor is already at the end
    // get the cursor position
    file_size = ftell(file);
    // get back at the beginning
    fseek(file, 0L, SEEK_SET);

    // calculate the struct number there is in the file
    struct_amount = file_size / sizeof(Save_Player_Score);
    printf("F: %llu\n", struct_amount);

    for (u64 count = 0; count * sizeof(Save_Player_Score) < file_size; count++) {
        // move the cursor as we read
        fread(&temp, sizeof(temp), 1, file);
        printf("player in score: %s is a biatch\n", temp.player_name);

        // check if the name is the same
        if (strcmp(temp.player_name, file_struct.player_name) == 0) {
            printf("found zi player %s\n", temp.player_name);
            // get back just before the wanted struct
            fseek(file, (long)(-sizeof(Save_Player_Score)), SEEK_CUR);
            // update the found struct
            file_struct.treasure_found += temp.treasure_found;
            file_struct.game_won += temp.game_won;
            file_struct.monster_killed += temp.monster_killed;
            // write the new data
            fwrite(&file_struct, sizeof(Save_Player_Score), 1, file);
            fclose(file);
            return true;
        }
    }

    printf("noot found zi player %s\n", file_struct.player_name);
    // if we are here, there is no one with the same name, so add it on the file
    fwrite(&file_struct, sizeof(Save_Player_Score), 1, file);

    // close the stream
    fclose(file);
    return true;
}

void show_score(u32 amount_to_show) {
    Save_Player_Score temp;
    u64 file_size;
    u64 struct_amount;

    FILE *file = fopen(SAVE_FOLDER SCORE_FILE_NAME, "rb");
    if (file == NULL) {
        printf("Failed to get the save file in show_score.\n");
    }

    // get the file size
    // move to the end of the file
    fseek(file, 0L, SEEK_END);
    // get the cursor position
    file_size = ftell(file);
    // get back at the beginning
    fseek(file, 0L, SEEK_SET);

    // calculate the struct number there is in the file
    struct_amount = file_size / sizeof(Save_Player_Score);
    if (struct_amount < amount_to_show) {
        amount_to_show = struct_amount;
    }

    for (u64 count = 0; count < amount_to_show; count++) {
        // move the cursor as we read
        fread(&temp, sizeof(temp), 1, file);

        // print the score
        printf("%s: %d\n", temp.player_name,
               temp.game_won * SCORE_WIN + temp.monster_killed * SCORE_KILL + temp.treasure_found * SCORE_TREASURE);
    }

    // close the stream
    fclose(file);
}