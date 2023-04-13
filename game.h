#pragma once

#include "./defines.h"

#define MAP_SIZE 7
#define SAVE_FILE_NAME "memory.save"
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
    PLAYER_GREEN,
    PLAYER_BLUE,
    PLAYER_WHITE,
    PLAYER_YELLOW
} Case_Type;

typedef enum Choosen_Weapon {
    WEAPON_TORCH,
    WEAPON_SHIELD,
    WEAPON_BOW,
    WEAPON_AXE
} Choosen_Weapon;

typedef struct Board_Case {
    // if the case has been discovered or not
    b8 hidden;
    // case outside of the map (outside square)
    b8 empty;
    // content of the case
    Case_Type content;
} Board_Case;

// create the 2D array of the map
Board_Case **map_create();
// free resources of the map
void map_destroy(Board_Case **map);
// reset the map for a new player round
void map_reset(Board_Case **map);
// print the map
void map_print();
//  move the current player
void player_move(Board_Case **map, Case_Type turn, Choosen_Weapon weapon);