#ifndef HEADER_MENU
#define HEADER_MENU 1

// nouvelle partie, sauvegarde --> charger la partie, montrer les scores, quitter le jeu
#include "./defines.h"
#include "game.h"

typedef enum Menu_Type {
    MENU_MAIN,
    MENU_GAME,
    MENU_LOAD_GAME,
    MENU_NEW_GAME,
    MENU_SCORE,
    MENU_QUIT,

    // for errors
    MENU_UNKNOWN
} Menu_Type;

void new_game(u32 *player_number, char player_name[MAX_PLAYER][PLAYER_NAME_LENGTH], Class_Type player_class[MAX_PLAYER]);
void reset_variables(u32 *player_number, u32 treasure_found[MAX_PLAYER], u32 monster_killed[MAX_PLAYER], u32 round_number[MAX_PLAYER],
                     u32 treasure[MAX_PLAYER], b8 will_teleport[MAX_PLAYER], b8 artifact_found[MAX_PLAYER],
                     u32 player_x[MAX_PLAYER], u32 player_y[MAX_PLAYER], Choosen_Weapon active_weapon[MAX_PLAYER], Class_Type player_class[MAX_PLAYER],
                     char player_name[MAX_PLAYER][PLAYER_NAME_LENGTH], Case_Type *turn, b8 *is_winner);
#endif