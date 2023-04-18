#ifndef HEADER_SAVE
#define HEADER_SAVE 1

#include "./defines.h"
#include "./game.h"

typedef struct Save_File_Struct {
    Board_Case **map;
    Case_Type turn;
    Choosen_Weapon weapon;
} Save_File_Struct;

b8 remove_save();
b8 save_file_exists();
b8 save_game(Board_Case **map, Case_Type turn, Choosen_Weapon weapon);
b8 load_game(Board_Case ***map, Case_Type *turn, Choosen_Weapon *weapon);
#endif