#ifndef HEADER_SAVE
#define HEADER_SAVE 1

#include "./defines.h"
#include "./game.h"

#define SAVE_FOLDER "save/"
#define SAVE_FILE_NAME "memory.save"
#define SCORE_FILE_NAME "score.save"

typedef struct Save_File_Struct {
    Board_Case map[MAP_SIZE][MAP_SIZE];
    u32 player_number;
    u32 treasure_found[MAX_PLAYER];
    u32 monster_killed[MAX_PLAYER];
    u32 round_number[MAX_PLAYER];
    u32 treasure[MAX_PLAYER];
    b8 will_teleport[MAX_PLAYER];
    b8 artifact_found[MAX_PLAYER];
    u32 player_x[MAX_PLAYER];
    u32 player_y[MAX_PLAYER];
    Choosen_Weapon active_weapon;
    Class_Type player_class[MAX_PLAYER];
    char player_name[MAX_PLAYER][PLAYER_NAME_LENGTH];
    Case_Type turn;
    b8 is_winner;
} Save_File_Struct;

// the file will be the same name as the player name
typedef struct Save_Player_Score {
    char player_name[PLAYER_NAME_LENGTH];
    u32 treasure_found;
    u32 monster_killed;
    u32 game_won;
} Save_Player_Score;

// remove the save file
b8 remove_save();
// check if the save file exists
b8 save_file_exists(const char *path);
// save the game in the save file
b8 save_game(Board_Case **map, u32 player_number, u32 treasure_found[MAX_PLAYER], u32 monster_killed[MAX_PLAYER], u32 round_number[MAX_PLAYER],
             u32 treasure[MAX_PLAYER], b8 will_teleport[MAX_PLAYER], b8 artifact_found[MAX_PLAYER],
             u32 player_x[MAX_PLAYER], u32 player_y[MAX_PLAYER], Choosen_Weapon active_weapon, Class_Type player_class[MAX_PLAYER],
             char player_name[MAX_PLAYER][PLAYER_NAME_LENGTH], Case_Type turn, b8 is_winner);
// load the game from the save file if it exists
b8 load_game(Board_Case **map, u32 *player_number, u32 treasure_found[MAX_PLAYER], u32 monster_killed[MAX_PLAYER], u32 round_number[MAX_PLAYER],
             u32 treasure[MAX_PLAYER], b8 will_teleport[MAX_PLAYER], b8 artifact_found[MAX_PLAYER],
             u32 player_x[MAX_PLAYER], u32 player_y[MAX_PLAYER], Choosen_Weapon *active_weapon, Class_Type player_class[MAX_PLAYER],
             char player_name[MAX_PLAYER][PLAYER_NAME_LENGTH], Case_Type *turn, b8 *is_winner);
// save the score into the score save file
b8 save_score(char player_name[PLAYER_NAME_LENGTH], u32 treasure_found, u32 monster_killed, u32 game_won);
#endif