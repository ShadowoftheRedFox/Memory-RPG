#ifndef HEADER_GAME
#define HEADER_GAME 1

#include "./defines.h"

#define MAP_SIZE 7
#define PLAYER_NAME_LENGTH 30
#define MAX_PLAYER 4
// VVV comment line below to disable debuggin VVV
// #define DEBUG

// define what is in a case
typedef enum Case_Type {
    // nothing in the case
    CASE_EMPTY,

    // different type of player spawn
    CASE_SPAWN_GREEN,
    CASE_SPAWN_BLUE,
    CASE_SPAWN_WHITE,
    CASE_SPAWN_YELLOW,

    // type of object
    CASE_OBJECT_TREASURE,
    CASE_OBJECT_PORTAL,
    CASE_OBJECT_TOTEM,

    // type of artifacts
    CASE_OBJECT_STAFF,
    CASE_OBJECT_DAGGER,
    CASE_OBJECT_GRIMOIRE,
    CASE_OBJECT_SWORD,

    // type of monster
    CASE_MONSTER_ZOMBIE,
    CASE_MONSTER_HARPY,
    CASE_MONSTER_BASILIC,
    CASE_MONSTER_TROLL,

    // player turn here for simplification
    PLAYER_BLUE,
    PLAYER_GREEN,
    PLAYER_WHITE,
    PLAYER_YELLOW,

    // for errors
    TYPE_UNKNOWN
} Case_Type;

typedef enum Class_Type {
    CLASS_MAGICIAN,
    CLASS_THIEF,
    CLASS_RANGER,
    CLASS_WARRIOR,

    // for errors
    CLASS_UNKNOWN
} Class_Type;

typedef enum Case_Color {
    COLOR_EMPTY,
    COLOR_BLUE,
    COLOR_YELLOW,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_WHITE,
    COLOR_ORANGE,
    COLOR_PINK,

    // for errors
    COLOR_UNKNOWN
} Case_Color;

typedef enum Choosen_Weapon {
    WEAPON_TORCH,
    WEAPON_SHIELD,
    WEAPON_BOW,
    WEAPON_AXE,

    // for errors
    WEAPON_UNKNOWN
} Choosen_Weapon;

typedef struct Board_Case {
    // if the case has been discovered or not
    b8 hidden;
    // case outside of the map (outside square)
    b8 empty;
    // content of the case
    Case_Type content;
    Case_Type player;
} Board_Case;

// create the 2D array of the map
Board_Case **map_create();
// free resources of the map
void map_destroy(Board_Case **map);
// reset the map for a new player round
void map_reset(Board_Case **map);
// create a new map for a new game
void map_setup(Board_Case **map);
// print the map
void map_print(Board_Case **map);
// do logic depending of the current case
void game_logic(Board_Case **map, Case_Type *turn, u32 *treasure_found, u32 *monster_killed,
                b8 *is_artifact_found, Class_Type player_class, Choosen_Weapon active_weapon,
                u32 player_amount, b8 *player_will_teleport,
                u32 *player_position_x, u32 *player_position_y, b8 *is_winner);
// change the turn to the next player
void game_next_turn(Board_Case **map, Case_Type *turn, u32 player_amount, u32 *player_position_x, u32 *player_position_y);
// switch a case choosen by the player with the totem case
void game_transmut(Board_Case **map);
// choose a weapon before each round
void game_choose_weapon(Choosen_Weapon *weapon);
// choose a class before each start of the game
void game_choose_class(Class_Type player_class[4], u32 count);
// win display and ask for anotehr game, also save
void game_win(Case_Type turn, char player_name[PLAYER_NAME_LENGTH], u32 round_number);

// fix the player image to it's coordinates and delete all old images
void move_player_image(Board_Case **map, u32 player_position_x, u32 player_position_y, Case_Type turn);

// teleport the player on any hidden case he wants
void player_teleport(Board_Case **map, Case_Type *turn, u32 *player_position_x, u32 *player_position_y);
//  move the current player
void player_move(Board_Case **map, Case_Type *turn, u32 player_amount, u32 *player_position_x, u32 *player_position_y,
                 u32 *treasure_found, b8 *is_artifact_found);

void empty_stdin_buffer();

#endif