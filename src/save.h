#ifndef HEADER_SAVE
#define HEADER_SAVE 1

#include "./defines.h"
#include "./game.h"

#define SAVE_FOLDER "save/"
#define SAVE_FILE_NAME "memory.save"
#define SCORE_FILE_NAME "score.save"

typedef struct Save_File_Struct {
    Board_Case **map;
    Case_Type turn;
    Choosen_Weapon weapon;
} Save_File_Struct;

b8 remove_save();
b8 save_file_exists();
b8 save_game(Board_Case **map, Case_Type turn, Choosen_Weapon weapon, u8 treasure_found, b8 artifact_found);
b8 load_game(Board_Case ***map, Case_Type *turn, Choosen_Weapon *weapon, u8 *treasure_found, b8 *artifact_found);
#endif