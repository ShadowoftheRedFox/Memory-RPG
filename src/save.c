#include "./save.h"
#include "./game.h"
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
    if (player_number == NULL || *player_number < 0 || *player_number > MAX_PLAYER) {
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
    if (active_weapon == NULL || *active_weapon < 0 || *active_weapon >= WEAPON_UNKNOWN) {
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
    if (turn == NULL || *turn < PLAYER_BLUE || *turn >= TYPE_UNKNOWN) {
        printf("turn is out of range in load_game\n");
        exit(1);
    }
    if (is_winner == NULL || *is_winner != true && *is_winner != false) {
        printf("is_winner is out of range in load_game\n");
        exit(1);
    }

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