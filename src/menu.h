#ifndef HEADER_MENU
#define HEADER_MENU 1

// nouvelle partie, sauvegarde --> charger la partie, montrer les scores, quitter le jeu
#include "defines.h"
#include "game.h"
#include "save.h"
#include "sort.h"

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

typedef enum Score_Menu_Type {
    SCORE_MAIN,
    SCORE_GAME,
    SCORE_LOAD_GAME,
    SCORE_NEW_GAME,
    SCORE_SCORE,
    SCORE_QUIT,

    // for errors
    SCORE_UNKNOWN
} Score_Menu_Type;

// prepare a new game, ask for player number, player name and class
void new_game(u32 *player_number, char player_name[MAX_PLAYER][PLAYER_NAME_LENGTH], Class_Type player_class[MAX_PLAYER]);
// set all variables to the startup state
void reset_variables(u32 *player_number, u32 treasure_found[MAX_PLAYER], u32 monster_killed[MAX_PLAYER], u32 round_number[MAX_PLAYER],
                     u32 treasure[MAX_PLAYER], b8 will_teleport[MAX_PLAYER], b8 artifact_found[MAX_PLAYER],
                     u32 player_x[MAX_PLAYER], u32 player_y[MAX_PLAYER], Choosen_Weapon active_weapon[MAX_PLAYER], Class_Type player_class[MAX_PLAYER],
                     char player_name[MAX_PLAYER][PLAYER_NAME_LENGTH], Case_Type *turn, b8 *is_winner);
// display score as we want
void score_menu();
// we will suppose that there is not enough player to display to think about handling items well the amount of RAM taken
void show_score(Sort_Type type);
// wait for a user interaction
void confirm();
// animate the printf
void animate_printf(const char *text);
#endif