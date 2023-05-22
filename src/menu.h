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

#endif