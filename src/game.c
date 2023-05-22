#include "./game.h"
#include "./platform.h"

// TODO add sleep and console clear

Board_Case **map_create() {
    // here malloc(size); = platform_allocate(size);
    // we alloc columns here
    Board_Case **map = platform_allocate(MAP_SIZE * sizeof(Board_Case *));
    // the test if malloc failed is in the function

    // we alloc lines here
    for (u8 i = 0; i < MAP_SIZE; i++) {
        map[i] = platform_allocate(MAP_SIZE * sizeof(Board_Case));
    }

    // fill the map with random order
    map_setup(map);

    return map;
}

void content_randomizer(Board_Case **map, u8 n, Case_Type content) {
    // verify parameters
    if (map == NULL) {
        printf("map is null in map_randomizer\n");
        exit(1);
    }
    if (content >= TYPE_UNKNOWN || content < 0) {
        printf("content is not the expected value in content_randomizer: %d\n",
               content);
        exit(1);
    }
    // since it's recursiv, stop if n == 0
    if (n == 0) {
        return;
    }

    // array of empty cases
    // zero memory because zero will be exluded from cases
    u8 count = 0;
    u8 *array = platform_allocate(sizeof(u8) * (MAP_SIZE - 2) * (MAP_SIZE - 2));
    platform_zero_memory(array, sizeof(u8) * (MAP_SIZE - 2) * (MAP_SIZE - 2));

    for (u8 y = 1; y <= MAP_SIZE - 2; y++) {
        for (u8 x = 1; x <= MAP_SIZE - 2; x++) {
            if (map[y][x].content == CASE_EMPTY) {
                // optimize space by using y * 10 + x
                array[count] = y * 10 + x;
                count++;
            }
        }
    }

    if (count != 0) {
        // get a random number between 0 and count
        u8 rand_count = rand() % count;
        u8 x = array[rand_count] % 10; // our x coordinate
        u8 y = array[rand_count] / 10; // our y coordinate
        free(array);

        // and fill it accordingly
        map[y][x].content = content;
        // then call again
        // not really effective, but it does the work, and we are not focused on
        // optimisation
        content_randomizer(map, n - 1, content);
    } else {
        free(array);
    }
}

// reset the map for a new player round
void map_setup(Board_Case **map) {
    // verify parameters
    if (map == NULL) {
        printf("map is null in map_reset\n");
        exit(1);
    }

    // fill the map
    for (u8 y = 0; y < MAP_SIZE; y++) {
        for (u8 x = 0; x < MAP_SIZE; x++) {
            // our current case is map[y][x]
            // setup the default content
            map[y][x].content = CASE_EMPTY;
            map[y][x].player = CASE_EMPTY;

            // case that are empty (they are the outside walls)
            if (y == 0 || x == 0 || x == MAP_SIZE - 1 || y == MAP_SIZE - 1) {
                map[y][x].empty = true;
                map[y][x].hidden = false;
            } else {
                // case hidden
                map[y][x].empty = false;
                map[y][x].hidden = true;
            }

            // set the player spawn
            if (x == 0 && y == 2) {
                map[y][x].empty = false;
                map[y][x].hidden = false;
                map[y][x].content = CASE_SPAWN_BLUE;
                map[y][x].player = PLAYER_BLUE;
            } else if (x == 4 && y == 0) {
                map[y][x].empty = false;
                map[y][x].hidden = false;
                map[y][x].content = CASE_SPAWN_GREEN;
                map[y][x].player = PLAYER_GREEN;
            } else if (x == 2 && y == MAP_SIZE - 1) {
                map[y][x].empty = false;
                map[y][x].hidden = false;
                map[y][x].content = CASE_SPAWN_YELLOW;
                map[y][x].player = PLAYER_YELLOW;
            } else if (x == MAP_SIZE - 1 && y == 4) {
                map[y][x].empty = false;
                map[y][x].hidden = false;
                map[y][x].content = CASE_SPAWN_WHITE;
                map[y][x].player = PLAYER_WHITE;
            }
        }
    }

    // randomisation of the map content
    // monsters
    content_randomizer(map, 4, CASE_MONSTER_BASILIC);
    content_randomizer(map, 4, CASE_MONSTER_HARPY);
    content_randomizer(map, 4, CASE_MONSTER_TROLL);
    content_randomizer(map, 4, CASE_MONSTER_ZOMBIE);
    // treasures
    content_randomizer(map, 2, CASE_OBJECT_TREASURE);
    // artifacts
    content_randomizer(map, 1, CASE_OBJECT_STAFF);
    content_randomizer(map, 1, CASE_OBJECT_DAGGER);
    content_randomizer(map, 1, CASE_OBJECT_GRIMOIRE);
    content_randomizer(map, 1, CASE_OBJECT_SWORD);
    // special
    content_randomizer(map, 2, CASE_OBJECT_TOTEM);
    content_randomizer(map, 1, CASE_OBJECT_PORTAL);
}

void map_reset(Board_Case **map) {
    // verify parameters
    if (map == NULL) {
        printf("map is null in map_reset\n");
        exit(1);
    }

    // fill the map
    for (u8 y = 0; y < MAP_SIZE; y++) {
        for (u8 x = 0; x < MAP_SIZE; x++) {
            // our current case is map[y][x]
            // setup the default content
            map[y][x].player = CASE_EMPTY;

            if (y == 0 || x == 0 || x == MAP_SIZE - 1 || y == MAP_SIZE - 1) {
                // case that are empty
                map[y][x].empty = true;
            } else {
                // case hidden
                map[y][x].empty = false;
                map[y][x].hidden = true;
            }

            // reset the player spawn
            if (x == 0 && y == 2) {
                map[y][x].empty = false;
                map[y][x].hidden = false;
                map[y][x].content = CASE_SPAWN_BLUE;
                map[y][x].player = PLAYER_BLUE;
            } else if (x == 4 && y == 0) {
                map[y][x].empty = false;
                map[y][x].hidden = false;
                map[y][x].content = CASE_SPAWN_GREEN;
                map[y][x].player = PLAYER_GREEN;
            } else if (x == 2 && y == MAP_SIZE - 1) {
                map[y][x].empty = false;
                map[y][x].hidden = false;
                map[y][x].content = CASE_SPAWN_YELLOW;
                map[y][x].player = PLAYER_YELLOW;
            } else if (x == MAP_SIZE - 1 && y == 4) {
                map[y][x].empty = false;
                map[y][x].hidden = false;
                map[y][x].content = CASE_SPAWN_WHITE;
                map[y][x].player = PLAYER_WHITE;
            }
        }
    }
}

// print the map
void map_print(Board_Case **map) {
    // verify parameters
    if (map == NULL) {
        printf("map is null in map_print\n");
        exit(1);
    }

    // loop through every case
    for (u8 y = 0; y < MAP_SIZE; y++) {
        for (u8 x = 0; x < MAP_SIZE; x++) {
            if (map[y][x].empty && map[y][x].player == CASE_EMPTY) {
                // empty case
                if (y == 0) {
                    printf("  ");
                } else {
                    printf("  ");
                }
            } else if (map[y][x].hidden) {
                // if case is hidden
                platform_color_change(COLOR_EMPTY, COLOR_EMPTY);
#ifdef PLATFORM_LINUX
                printf("░░");
#else
                printf("HH");
#endif
            } else {
                // player is in map[y][x].player
                switch (map[y][x].player) {
#ifdef PLATFORM_LINUX
                //  player turn here for simplification
                case PLAYER_GREEN:
                    platform_color_change(COLOR_GREEN, COLOR_EMPTY);
                    printf("⟏"); // warrior
                    break;
                case PLAYER_BLUE:
                    platform_color_change(COLOR_BLUE, COLOR_EMPTY);
                    printf("⤅"); // ranger
                    break;
                case PLAYER_WHITE:
                    platform_color_change(COLOR_EMPTY, COLOR_EMPTY);
                    printf("⚿"); // thief
                    break;
                case PLAYER_YELLOW:
                    platform_color_change(COLOR_YELLOW, COLOR_EMPTY);
                    // platform_color_change(COLOR_YELLOW, COLOR_EMPTY);
                    printf("⧋"); // wizard
                    break;
#else
                //  player turn here for simplification
                case PLAYER_GREEN:
                    platform_color_change(COLOR_GREEN, COLOR_EMPTY);
                    printf("GR");
                    break;
                case PLAYER_BLUE:
                    platform_color_change(COLOR_BLUE, COLOR_EMPTY);
                    printf("BL");
                    break;
                case PLAYER_WHITE:
                    platform_color_change(COLOR_EMPTY, COLOR_EMPTY);
                    printf("WH");
                    break;
                case PLAYER_YELLOW:
                    platform_color_change(COLOR_YELLOW, COLOR_EMPTY);
                    printf("YE");
                    break;
#endif
                default:
                    switch (map[y][x].content) {
#ifdef PLATFORM_LINUX
                    // type of object
                    case CASE_OBJECT_TREASURE:
                        platform_color_change(COLOR_YELLOW, COLOR_EMPTY);
                        printf("﹩");
                        break;
                    case CASE_OBJECT_PORTAL:
                        platform_color_change(COLOR_PINK, COLOR_EMPTY);
                        printf("⦿");
                        break;
                    case CASE_OBJECT_TOTEM:
                        platform_color_change(COLOR_PINK, COLOR_EMPTY);
                        printf("🞖");
                        break;
                    case CASE_OBJECT_STAFF:
                        platform_color_change(COLOR_GREEN, COLOR_EMPTY);
                        printf("Ⳕ");
                        break;
                    case CASE_OBJECT_DAGGER:
                        platform_color_change(COLOR_EMPTY, COLOR_EMPTY);
                        printf("𐃋");
                        break;
                    case CASE_OBJECT_GRIMOIRE:
                        platform_color_change(COLOR_BLUE, COLOR_EMPTY);
                        printf("🕮");
                        break;
                    case CASE_OBJECT_SWORD:
                        platform_color_change(COLOR_ORANGE, COLOR_EMPTY);
                        printf("⚔");
                        break;

                    // type of monster
                    case CASE_MONSTER_ZOMBIE:
                        platform_color_change(COLOR_RED, COLOR_EMPTY);
                        printf("🕱");
                        break;
                    case CASE_MONSTER_HARPY:
                        platform_color_change(COLOR_ RED, COLOR_EMPTY);
                        printf("⊛");
                        break;
                    case CASE_MONSTER_BASILIC:
                        platform_color_change(COLOR_RED, COLOR_EMPTY);
                        printf("𝧜"); // don't know if this one works
                        break;
                    case CASE_MONSTER_TROLL:
                        platform_color_change(COLOR_RED, COLOR_EMPTY);
                        printf("🧌"); // don't know if this one works
                        break;

                        // different type of player spawn
                    case CASE_SPAWN_GREEN:
                        platform_color_change(COLOR_GREEN, COLOR_EMPTY);
                        printf("▧");
                        break;
                    case CASE_SPAWN_BLUE:
                        platform_color_change(COLOR_BLUE, COLOR_EMPTY);
                        printf("▧");
                        break;
                    case CASE_SPAWN_WHITE:
                        platform_color_change(COLOR_WHITE, COLOR_EMPTY);
                        printf("▧");
                        break;
                    case CASE_SPAWN_YELLOW:
                        platform_color_change(COLOR_YELLOW, COLOR_EMPTY);
                        printf("▧");
                        break;
#else
                    // type of object
                    case CASE_OBJECT_TREASURE:
                        platform_color_change(COLOR_YELLOW, COLOR_EMPTY);
                        printf("TT");
                        break;
                    case CASE_OBJECT_PORTAL:
                        platform_color_change(COLOR_PINK, COLOR_EMPTY);
                        printf("PP");
                        break;
                    case CASE_OBJECT_TOTEM:
                        platform_color_change(COLOR_PINK, COLOR_EMPTY);
                        printf("TO");
                        break;
                    case CASE_OBJECT_STAFF:
                        platform_color_change(COLOR_GREEN, COLOR_EMPTY);
                        printf("SS");
                        break;
                    case CASE_OBJECT_DAGGER:
                        platform_color_change(COLOR_EMPTY, COLOR_EMPTY);
                        printf("DD");
                        break;
                    case CASE_OBJECT_GRIMOIRE:
                        platform_color_change(COLOR_BLUE, COLOR_EMPTY);
                        printf("GG");
                        break;
                    case CASE_OBJECT_SWORD:
                        platform_color_change(COLOR_ORANGE, COLOR_EMPTY);
                        printf("SW");
                        break;

                    // type of monster
                    case CASE_MONSTER_ZOMBIE:
                        platform_color_change(COLOR_RED, COLOR_EMPTY);
                        printf("ZO");
                        break;
                    case CASE_MONSTER_HARPY:
                        platform_color_change(COLOR_RED, COLOR_EMPTY);
                        printf("HA");
                        break;
                    case CASE_MONSTER_BASILIC:
                        platform_color_change(COLOR_RED, COLOR_EMPTY);
                        printf("BA"); // don't know if this one works
                        break;
                    case CASE_MONSTER_TROLL:
                        platform_color_change(COLOR_RED, COLOR_EMPTY);
                        printf("TR"); // don't know if this one works
                        break;

                        // different type of player spawn
                    case CASE_SPAWN_GREEN:
                        platform_color_change(COLOR_GREEN, COLOR_EMPTY);
                        printf("OO"); // warrior
                        break;
                    case CASE_SPAWN_BLUE:
                        platform_color_change(COLOR_BLUE, COLOR_EMPTY);
                        printf("OO"); // ranger
                        break;
                    case CASE_SPAWN_WHITE:
                        platform_color_change(COLOR_WHITE, COLOR_EMPTY);
                        printf("OO"); // thief
                        break;
                    case CASE_SPAWN_YELLOW:
                        platform_color_change(COLOR_YELLOW, COLOR_EMPTY);
                        printf("OO"); // wizard
                        break;
#endif
                    default:
                        // supposed that the only things here is case empty
                        platform_color_change(COLOR_EMPTY, COLOR_EMPTY);
                        printf("  ");
                        break;
                    }
                    break;
                }
            }
        }
        platform_color_change(COLOR_EMPTY, COLOR_EMPTY);
        printf("\n");
    }
    printf("\n");
}

//  move the current player
void player_move(Board_Case **map, Case_Type *turn, u32 player_amount,
                 u32 *player_position_x, u32 *player_position_y,
                 u32 *treasure_found, b8 *is_artifact_found) {
    // verify parameters
    if (map == NULL) {
        printf("map is null in player_move\n");
        exit(1);
    }
    if (turn == NULL || *turn >= TYPE_UNKNOWN || *turn < PLAYER_BLUE) {
        printf("turn is invalid in player_move\n");
        exit(1);
    }
    if (player_position_x == NULL) {
        printf("player_position_x is null in player_move\n");
        exit(1);
    }
    if (player_position_y == NULL) {
        printf("player_position_y is null in player_move\n");
        exit(1);
    }
    if (player_amount > 4) {
        printf("player_amount is out of range in player_move\n");
        exit(1);
    }

    // Choose the direction
    i32 choosen_direction;
    i32 correct = 0;
    b8 moved = false;

    // check if player can move on at least one case
    if ((*player_position_y > 0 &&
         map[*player_position_y - 1][*player_position_x].hidden == false) &&
        (*player_position_y < MAP_SIZE - 1 &&
         map[*player_position_y + 1][*player_position_x].hidden == false) &&
        (*player_position_x > 0 &&
         map[*player_position_y][*player_position_x - 1].hidden == false) &&
        (*player_position_x < MAP_SIZE - 1 &&
         map[*player_position_y][*player_position_x + 1].hidden == false)) {
        printf("You can't move anywhere!\n");
        // change turn and reset map
        *treasure_found = 0;
        is_artifact_found = false;
        game_next_turn(map, turn, player_amount, player_position_x,
                       player_position_y);
        return;
    }

    do {
        printf("Choose your path: \n   [8]   \n[4]   [6]\n   [2]   \n>> ");
        correct = scanf("%d", &choosen_direction);
        empty_stdin_buffer();
        switch (choosen_direction) {
        case 2: // down
            if (*player_position_y >= 5 || *player_position_x == 0 ||
                *player_position_x == 6 ||
                map[*player_position_y + 1][*player_position_x].hidden == false) {
                printf("You can't move down here!\n");
                break;
            }
            *player_position_y += 1;
            map[*player_position_y][*player_position_x].hidden = false;
            map_print(map);
            moved = true;
            break;
        case 4: // left
            if (*player_position_x <= 1 || *player_position_y == 0 ||
                *player_position_y == 6 ||
                map[*player_position_y][*player_position_x - 1].hidden == false) {
                printf("You can't move left here!\n");
                break;
            }
            *player_position_x -= 1;
            map[*player_position_y][*player_position_x].hidden = false;
            map_print(map);
            moved = true;
            break;
        case 6: // right
            if (*player_position_x >= 5 || *player_position_y == 0 ||
                *player_position_y == 6 ||
                map[*player_position_y][*player_position_x + 1].hidden == false) {
                printf("You can't move right here!\n");
                break;
            }
            *player_position_x += 1;
            map[*player_position_y][*player_position_x].hidden = false;
            map_print(map);
            moved = true;
            break;
        case 8: // up
            if (*player_position_y <= 1 || *player_position_x == 0 ||
                *player_position_x == 6 ||
                map[*player_position_y - 1][*player_position_x].hidden == false) {
                printf("You can't move up here!\n");
                break;
            }
            *player_position_y -= 1;
            map[*player_position_y][*player_position_x].hidden = false;
            map_print(map);
            moved = true;
            break;
        default:
            printf("This is not a correct direction!\n");
            break;
        }
    } while (correct != 1 || moved == false);
}

void player_teleport(Board_Case **map, Case_Type *turn, u32 *player_position_x,
                     u32 *player_position_y) {
    // verify parameters
    if (map == NULL) {
        printf("map is null in player_teleport\n");
        exit(1);
    }
    // BUG we don't need the turn?
    if (turn == NULL || *turn >= TYPE_UNKNOWN || *turn < PLAYER_BLUE) {
        printf("turn is invalid in player_teleport\n");
        exit(1);
    }
    if (player_position_x == NULL) {
        printf("player_position_x is null in player_teleport\n");
        exit(1);
    }
    if (player_position_y == NULL) {
        printf("player_position_y is null in player_teleport\n");
        exit(1);
    }

    printf("Choose somewhere to be teleported:\n");

    // will loop throught all case from above
    for (u32 y = 0; y <= (MAP_SIZE - 2); y++) {
        for (u32 x = 0; x <= (MAP_SIZE - 2); x++) {
            // if the case is hidden, print it's coordinates
            if (map[y][x].hidden) {
                printf("%02d ", (y * 10 + x));
            } else {
                printf("   ");
            }
        }
        printf("\n");
    }

    // ask the player the new coordinates
    i32 correct = 0;
    i32 case_input = -1;
    do {
        printf(">> ");
        correct = scanf("%d", &case_input);
        empty_stdin_buffer();
    } while (correct != 1 ||
             map[(case_input) / 10][(case_input) % 10].hidden == false);

    // the player move the player to the new coordinates
    *player_position_x = (case_input) % 10;
    *player_position_y = (case_input) / 10;
    // hide the position you went on
    map[*player_position_y][*player_position_x].hidden = false;

    // print the map
    map_print(map);
}

void game_logic(Board_Case **map, Case_Type *turn, u32 *treasure_found,
                u32 *monster_killed, b8 *is_artifact_found,
                Class_Type player_class, Choosen_Weapon active_weapon,
                u32 player_amount, b8 *player_will_teleport,
                u32 *player_position_x, u32 *player_position_y, b8 *is_winner) {
    // verify parameters
    if (map == NULL) {
        printf("map is null in game_logic\n");
        exit(1);
    }
    if (treasure_found == NULL) {
        printf("treasure_found is null in game_logic\n");
        exit(1);
    }
    if (monster_killed == NULL) {
        printf("monster_killed is null in game_logic\n");
        exit(1);
    }
    if (is_artifact_found == NULL) {
        printf("is_artifact_found is null in game_logic\n");
        exit(1);
    }
    if (player_will_teleport == NULL) {
        printf("player_will_teleport is null in game_logic\n");
        exit(1);
    }
    if (player_position_x == NULL || *player_position_x >= MAP_SIZE) {
        printf("player_position_x is out of range in game_logic\n");
        exit(1);
    }
    if (player_position_y == NULL || *player_position_y >= MAP_SIZE) {
        printf("player_position_y is out of range in game_logic\n");
        exit(1);
    }
    if (active_weapon >= WEAPON_UNKNOWN) {
        printf("active_weapon is out of range in game_logic\n");
        exit(1);
    }
    if (player_amount > 4 || player_amount <= 0) {
        printf("player_amount is out of range in game_logic\n");
        exit(1);
    }
    if (player_class >= CLASS_UNKNOWN) {
        printf("player_class is out of range in game_logic\n");
        exit(1);
    }
    if (turn == NULL || *turn >= TYPE_UNKNOWN || *turn < PLAYER_BLUE) {
        printf("turn is invalid in game_logic\n");
        exit(1);
    }

    // interaction for every case possible where the player is
    switch (map[*player_position_y][*player_position_x].content) {
        // type of object
    case CASE_OBJECT_TREASURE:
        if (*treasure_found == 0) {
            if (*is_artifact_found) {
                // enable the win display
                *is_winner = true;
            } else {
                printf("You found a treasure! Now find your artifact.\n");
                move_player_image(map, *player_position_x, *player_position_y, *turn);
                *treasure_found += 1;
            }
        } else {
            printf("You found another treasure! Now find your artifact.\n");
            move_player_image(map, *player_position_x, *player_position_y, *turn);
            *treasure_found += 1;
        }
        break;
    case CASE_OBJECT_PORTAL:
        printf("You found a weird shiny portal and you step inside...\n");
        move_player_image(map, *player_position_x, *player_position_y, *turn);
        *player_will_teleport = true;
        break;
    case CASE_OBJECT_TOTEM:
        move_player_image(map, *player_position_x, *player_position_y, *turn);
        game_transmut(map);
        *treasure_found = 0;
        is_artifact_found = false;
        game_next_turn(map, turn, player_amount, player_position_x,
                       player_position_y);
        break;
    case CASE_OBJECT_STAFF:
        if (player_class == CLASS_RANGER && !(*is_artifact_found)) {
            if (*treasure_found > 0) {
                // enable the win display
                *is_winner = true;
            } else {
                printf("You found the control staff you seeked for! Now find a treasure.\n");
                move_player_image(map, *player_position_x, *player_position_y, *turn);
                *is_artifact_found = true;
            }
        } else {
            printf("You found the control staff, but that's not what you wanted. Continue your journey.\n");
            move_player_image(map, *player_position_x, *player_position_y, *turn);
        }
        break;
    case CASE_OBJECT_DAGGER:
        if (player_class == CLASS_THIEF && !(*is_artifact_found)) {
            if (*treasure_found > 0) {
                // enable the win display
                *is_winner = true;
            } else {
                printf("You found the dagger you seeked for! Now find a treasure.\n");
                move_player_image(map, *player_position_x, *player_position_y, *turn);
                *is_artifact_found = true;
            }
        } else {
            printf("You found the dagger, but that's not what you wanted. Continue your journey.\n");
            move_player_image(map, *player_position_x, *player_position_y, *turn);
        }
        break;
    case CASE_OBJECT_GRIMOIRE:
        if (player_class == CLASS_MAGICIAN && !(*is_artifact_found)) {
            if (*treasure_found > 0) {
                // enable the win display
                *is_winner = true;
            } else {
                printf("You found the grimoire you seeked for! Now find a treasure.\n");
                move_player_image(map, *player_position_x, *player_position_y, *turn);
                *is_artifact_found = true;
            }
        } else {
            printf("You found the grimoire, but that's not what you wanted. Continue your journey.\n");
            move_player_image(map, *player_position_x, *player_position_y, *turn);
        }
        break;
    case CASE_OBJECT_SWORD:
        if (player_class == CLASS_WARRIOR && !(*is_artifact_found)) {
            if (*treasure_found > 0) {
                // enable the win display
                *is_winner = true;
            } else {
                printf("You found the sword you seeked for! Now find a treasure.\n");
                move_player_image(map, *player_position_x, *player_position_y, *turn);
                *is_artifact_found = true;
            }
        } else {
            printf("You found the sword, but that's not what you wanted. Continue your journey.\n");
            move_player_image(map, *player_position_x, *player_position_y, *turn);
        }
        break;

    // type of monster
    case CASE_MONSTER_ZOMBIE:
        if (active_weapon == WEAPON_TORCH) {
            printf("A zombie is in your way, but you burn him alive (or dead?) with your torch. Continue your journey.\n");
            move_player_image(map, *player_position_x, *player_position_y, *turn);
        } else {
            printf("A zombie is in your way, but you can't defeat him and he eats your brain.\n");
            *treasure_found = 0;
            is_artifact_found = false;
            game_next_turn(map, turn, player_amount, player_position_x,
                           player_position_y);
        }
        break;
    case CASE_MONSTER_HARPY:
        if (active_weapon == WEAPON_BOW) {
            printf("A harpy is in your way, but you kill it with your bow. Continue your journey.\n");
            move_player_image(map, *player_position_x, *player_position_y, *turn);
        } else {
            printf("A harpy is in you way, but you can't defeat it and it kills you.\n");
            *treasure_found = 0;
            is_artifact_found = false;
            game_next_turn(map, turn, player_amount, player_position_x,
                           player_position_y);
        }
        break;
    case CASE_MONSTER_BASILIC:
        if (active_weapon == WEAPON_SHIELD) {
            printf("A basilic is in your way, but it petrifies itself on the reflection of your shield. Continue your journey.\n");
            move_player_image(map, *player_position_x, *player_position_y, *turn);
        } else {
            printf("A basilic is in your way, but you can't defeat it and transforms you to a stone statue.\n");
            *treasure_found = 0;
            is_artifact_found = false;
            game_next_turn(map, turn, player_amount, player_position_x,
                           player_position_y);
        }
        break;
    case CASE_MONSTER_TROLL:
        if (active_weapon == WEAPON_AXE) {
            printf("A troll is in your way, you cut him in half with your axes. Continue your journey.\n");
            move_player_image(map, *player_position_x, *player_position_y, *turn);
        } else {
            printf("A troll is in your way, but you can't defeat him and it smashes you to thin air.\n");
            *treasure_found = 0;
            is_artifact_found = false;
            game_next_turn(map, turn, player_amount, player_position_x,
                           player_position_y);
        }
        break;
    case CASE_SPAWN_BLUE:
    case CASE_SPAWN_GREEN:
    case CASE_SPAWN_YELLOW:
    case CASE_SPAWN_WHITE:
        // hand the spawn, we do nothing
        break;
    default:
        printf("Unhandled case value: %d at (%d;%d)\n",
               map[*player_position_y][*player_position_x].content,
               *player_position_y, *player_position_x);
        break;
    }
}

void move_player_image(Board_Case **map, u32 player_position_x,
                       u32 player_position_y, Case_Type turn) {
    map[player_position_y][player_position_x].empty = true;
    // TODO can be optimized in a single loop
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            if (map[y][x].player == turn) {
                map[y][x].player = CASE_EMPTY;
                break;
            }
        }
    }
    // set the player image to the player coordinates
    map[player_position_y][player_position_x].player = turn;
}

void game_transmut(Board_Case **map) {
    // verify parameters
    if (map == NULL) {
        printf("map is null in game_transmut\n");
        exit(1);
    }

    printf("You found a transmutation totem.\nChoose a case to switch position "
           "with the totem.\n\n");

    // will loop throught all case from above
    for (u32 y = 0; y <= (MAP_SIZE - 2); y++) {
        for (u32 x = 0; x <= (MAP_SIZE - 2); x++) {
            // if the case is hidden, print it's coordinates
            // !case next to the player spawn can't be transmuted
            if (map[y][x].hidden && !(x == 1 && y == 2) && !(x == 4 && y == 1) &&
                !(x == 2 && y == MAP_SIZE - 2) && !(x == MAP_SIZE - 2 && y == 4)) {
                printf("%02d ", (y * 10 + x));
            } else {
                printf("   ");
            }
        }
        printf("\n");
    }

    // ask the player the new coordinates
    i32 correct = 0;
    u32 case_input = -1;
    do {
        printf(">> ");
        correct = scanf("%d", &case_input);
        empty_stdin_buffer();
    } while (correct != 1 ||
             map[(case_input) / 10][(case_input) % 10].hidden == false ||
             ((case_input) % 10 == 1 && (case_input) / 10 == 2) ||
             ((case_input) % 10 == 4 && (case_input) / 10 == 1) ||
             ((case_input) % 10 == 2 && (case_input) / 10 == MAP_SIZE - 2) ||
             ((case_input) % 10 == MAP_SIZE - 2 && (case_input) / 10 == 4));

    // switch the choosen case and the totem
    Board_Case temp;
    // TODO loop can be optimized
    for (u32 y = 0; y <= (MAP_SIZE - 2); y++) {
        for (u32 x = 0; x <= (MAP_SIZE - 2); x++) {
            if (map[y][x].content == CASE_OBJECT_TOTEM) {
                temp = map[y][x];
                map[y][x] = map[(case_input) / 10][(case_input) % 10];
                map[(case_input) / 10][(case_input) % 10] = temp;
                break;
            }
        }
    }

    printf("The totem has beenn switched with your choosen case!\n");
}

void game_next_turn(Board_Case **map, Case_Type *turn, u32 player_amount,
                    u32 *player_position_x, u32 *player_position_y) {
    // verify parameters
    if (map == NULL) {
        printf("map is null in game_next_turn\n");
        exit(1);
    }
    if (turn == NULL || *turn >= TYPE_UNKNOWN || *turn < PLAYER_BLUE) {
        printf("turn is invalid in game_next_turn\n");
        exit(1);
    }
    if (player_amount > 4) {
        printf("player_amout is out of range in game_next_turn\n");
        exit(1);
    }

    printf("This is the end of your turn.\n");

    // change the player coordinate to it's base
    switch (*turn) {
    case PLAYER_BLUE:
        *player_position_x = 0;
        *player_position_y = 2;
        break;
    case PLAYER_GREEN:
        *player_position_x = 4;
        *player_position_y = 0;
        break;
    case PLAYER_WHITE:
        *player_position_x = MAP_SIZE - 1;
        *player_position_y = 4;
        break;
    case PLAYER_YELLOW:
        *player_position_x = 2;
        *player_position_y = MAP_SIZE - 1;
        break;
    default:
        printf("unhandled value in game_next_turn: %d\n", *turn);
        break;
    }

    // we don't need to change turn if there is only one player
    printf("amnt: %d\nturn: %d\n", player_amount, *turn);
    if (player_amount > 1) {
        *turn += 1;
        if (*turn - PLAYER_BLUE >= player_amount) {
            *turn = PLAYER_BLUE;
        }
    }

    // change case to hidden and un empty correct case without randomizing it
    map_reset(map);
}

void game_win(Case_Type turn, char player_name[PLAYER_NAME_LENGTH],
              u32 round_number) {
    // verify parameters
    if (turn >= TYPE_UNKNOWN || turn < PLAYER_BLUE) {
        printf("turn is invalid in game_win\n");
        exit(1);
    }
    if (player_name == NULL) {
        printf("player_name is null in game_win\n");
        exit(1);
    }

    i32 correct = 0;
    i32 answer = 0;

    // TODO save the score into the score files

    printf("Well done %s! You won after %d rounds!\n", player_name, round_number);
}

void game_choose_weapon(Choosen_Weapon *weapon) {
    // verify parameters
    if (weapon == NULL) {
        printf("weapon is null in game_choose_weapon\n");
        exit(1);
    }

    u32 choice = 0;
    i32 correct = 0;

    printf("\nChoose your weapon wisely:\n1 - Torch\n2 - Shield\n3 - Bow\n4 - "
           "Axe\n\n");
    do {
        printf(">> ");
        correct = scanf("%d", &choice);
        empty_stdin_buffer();
    } while (choice >= 5 || choice <= 0 || correct != 1);

    *weapon = choice - 1;

    switch (*weapon) {
    case WEAPON_AXE:
        printf("You choose the axe, you're ready to cut in half trolls.\n");
        break;
    case WEAPON_BOW:
        printf("You choose the bow. Ready to shoot harpy ?\n");
        break;
    case WEAPON_TORCH:
        printf("You choose the torch, you're ready to burn to ash zombies.\n");
        break;
    case WEAPON_SHIELD:
        printf("You choose the shield. Ready to petrify the Basilic ?\n");
        break;
    default:
        printf("weapon is out of range in game_choose_weapon\n");
        break;
    }
}

void game_choose_class(Class_Type player_class[4], u32 count) {
    // verify parameters
    if (player_class == NULL) {
        printf("player_class is null in game_choose_class\n");
        exit(1);
    }
    if (count < 0 || count > MAX_PLAYER) {
        printf("count is out of range in game_choose_class\n");
        exit(1);
    }

    /*
    CLASS_MAGICIAN,
    CLASS_THIEF,
    CLASS_RANGER,
    CLASS_WARRIOR,

    CLASS_UNKNOWN
    */

    u32 choice = 0;
    i32 correct = 0;
    u8 free_choice_left = 4;
    Class_Type temp = CLASS_UNKNOWN;
    u32 min = 0;

    /*
    array of taken class (player_class) and array of free class
    check if any are taken in the taken array
    if yes, remove the class from the free class array
    make the taken class CLASS_UNKNOWN and drop them at the end
    then ask all those item in free
    */

    Class_Type free_class[MAX_PLAYER] = {CLASS_MAGICIAN, CLASS_THIEF, CLASS_RANGER, CLASS_WARRIOR};

    // loop through the taken array
    for (u32 t = 0; t < MAX_PLAYER; t++) {
        // if taken class if unknown, skip the check since it's useless
        if (player_class[t] == CLASS_UNKNOWN) {
            continue;
        } else {
            // loop through free array
            for (u32 f = 0; f < MAX_PLAYER; f++) {
                // we find a taken class, remove it from free class and check next taken
                // class
                if (player_class[t] == free_class[f]) {
                    free_class[f] = CLASS_UNKNOWN;
                    free_choice_left--;
                    break;
                }
            }
        }
    }

    // fall the non unknown class to the beginning of the free class (with selection sort cuz only 4 items)
    for (u32 i = 0; i < MAX_PLAYER; i++) {
        min = i;
        for (u32 j = i + 1; j < MAX_PLAYER; j++) {
            if (free_class[j] < free_class[min]) {
                min = j;
            }
        }

        temp = free_class[i];
        free_class[i] = free_class[min];
        free_class[min] = temp;
    }

    // if there is only one choice left, just assigned it and tell the player
    if (free_choice_left == 1) {
        printf("Sadly, you are the last player to choose your class. This is a one way choice...\n");
        player_class[count] = free_class[0];
    } else {
        // print ✨ beautifully ✨ the class choice
        printf("\nChoose your class wisely:\n");
        for (u32 i = 0; i < free_choice_left; i++) {
            printf("%d - ", i + 1);
            if (free_class[i] == CLASS_MAGICIAN) {
                printf("Magician\n");
            } else if (free_class[i] == CLASS_THIEF) {
                printf("Thief\n");
            } else if (free_class[i] == CLASS_RANGER) {
                printf("Ranger\n");
            } else if (free_class[i] == CLASS_WARRIOR) {
                printf("Warrior\n");
            }
        }

        do {
            printf(">> ");
            correct = scanf("%d", &choice);
            empty_stdin_buffer();
        } while (choice > free_choice_left || choice <= 0 || correct != 1);

        player_class[count] = free_class[choice - 1];
    }

    switch (player_class[count]) {
    case CLASS_MAGICIAN:
        printf("You choose the Magician. You will seek for the holy Grimoire, "
               "you're going to love casting those spells.\n");
        break;
    case CLASS_THIEF:
        printf("You choose the Thief. You will seek the sleeper's Dager, ready to "
               "'reclaim' what's 'yours'?\n");
        break;
    case CLASS_RANGER:
        printf("You choose the ranger. You will seek the powerful pet control "
               "stick, to form an army of cats.\n");
        break;
    case CLASS_WARRIOR:
        printf("You choose the warrior. You will seek for the mighty sword of "
               "fire, to bring warmness to your home.\n");
        break;
    default:
        printf("class is out of range in game_choose_class\n");
        break;
    }
}

void map_destroy(Board_Case **map) {
    // verify parameters
    if (map == NULL) {
        printf("map is null in map_destroy\n");
        exit(1);
    }

    // we unalloc lines here
    for (u8 i = 0; i < MAP_SIZE; i++) {
        platform_free(map[i]);
    }
    platform_free(map);
}

void empty_stdin_buffer() {
    while (getchar() != '\n') {
        // empty the stdin buffer until we found the next next line
    };
}