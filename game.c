#include "./game.h"
#include "./platform.h"

Board_Case **map_create() {
    // here malloc(size); = platform_allocate(size);
    // we alloc columns here
    Board_Case **map = platform_allocate(MAP_SIZE * sizeof(Board_Case *));
    // the test if malloc failed is in the function

    // we alloc lines here
    for (u8 i = 0; i < MAP_SIZE; i++) {
        map[i] = platform_allocate(MAP_SIZE * sizeof(Board_Case));
    }

    // kinda reset the map, well same as teh first setup
    map_reset(map);

    return map;
}

void content_randomizer(Board_Case **map, u8 n, Case_Type content) {
    // verify parameters
    if (map == NULL) {
        printf("Map is null on map_randomizer\n");
        exit(2);
    }
    if (content >= TYPE_UNKNOWN || content < 0) {
        printf("content is not the expected value: %d\n", content);
        exit(1);
    }
    // since it's recursiv, stop if n == 0
    if (n == 0) {
        return;
    }
    // array of empty cases
    // zero memory because zero will be exluded from cases
    u8 count = 0;
    u8 *array = platform_allocate(sizeof(u8) * MAP_SIZE);
    platform_zero_memory(array, sizeof(u8) * MAP_SIZE);

    for (u8 y = 1; y < MAP_SIZE - 2; y++) {
        for (u8 x = 1; x < MAP_SIZE - 2; x++) {
            if (map[y][x].content == CASE_EMPTY) {
                // optimize space by using y * 10 + x
                array[count] = y * 10 + x;
                count++;
            }
        }
    }

    // no empty case after count == 0?
    if (count != 0) {
        // get a random number between 0 and count
        u8 rand_count = rand() % count;
        u8 x = array[rand_count] % 10; // our x coordinate
        u8 y = array[rand_count] / 10; // our y coordinate

        // and fill it accordingly
        map[y][x].content = content;
        // then call again
        // not really effective, but it does the work, and we are not focused on optimisation
        content_randomizer(map, n - 1, content);
    }

    free(array);
}

// reset the map for a new player round
void map_reset(Board_Case **map) {
    // verify parameters
    if (map == NULL) {
        printf("Map is null on map_reset\n");
        exit(2);
    }
    // fill the map
    for (u8 y = 0; y < MAP_SIZE; y++) {
        for (u8 x = 0; x < MAP_SIZE; x++) {
            // our current case is map[y][x]
            if (y == 0 || x == 0 || x == MAP_SIZE - 1 || y == MAP_SIZE - 1) {
                // case that are empty
                map[y][x].empty = true;
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
            } else if (x == 4 && y == 0) {
                map[y][x].empty = false;
                map[y][x].hidden = false;
                map[y][x].content = CASE_SPAWN_GREEN;
            } else if (x == 2 && y == MAP_SIZE - 1) {
                map[y][x].empty = false;
                map[y][x].hidden = false;
                map[y][x].content = CASE_SPAWN_YELLOW;
            } else if (x == MAP_SIZE - 1 && y == 4) {
                map[y][x].empty = false;
                map[y][x].hidden = false;
                map[y][x].content = CASE_SPAWN_WHITE;
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

// print the map
void map_print(Board_Case **map) {
    // verify parameters
    if (map == NULL) {
        printf("Map is null on map_print\n");
        exit(2);
    }
    for (u8 y = 0; y < MAP_SIZE; y++) {
        for (u8 x = 0; x < MAP_SIZE; x++) {
            if (map[y][x].hidden) {
                // if case is hidden
                printf("░░");
            } else if (map[y][x].empty) {
                // empty case
                printf("  ");
            } else {
                switch (map[y][x].content) {
                    // different type of player spawn
                case CASE_SPAWN_GREEN:
                    printf("▧");
                    break;
                case CASE_SPAWN_BLUE:
                    printf("▧");
                    break;
                case CASE_SPAWN_WHITE:
                    printf("▧");
                    break;
                case CASE_SPAWN_YELLOW:
                    printf("▧");
                    break;

                // type of object
                case CASE_OBJECT_TREASURE:
                    printf("▧");
                    break;
                case CASE_OBJECT_PORTAL:
                    printf("▧");
                    break;
                case CASE_OBJECT_TOTEM:
                    printf("▧");
                    break;
                case CASE_OBJECT_STAFF:
                    printf("▧");
                    break;
                case CASE_OBJECT_DAGGER:
                    printf("▧");
                    break;
                case CASE_OBJECT_GRIMOIRE:
                    printf("▧");
                    break;
                case CASE_OBJECT_SWORD:
                    printf("▧");
                    break;

                // type of monster
                case CASE_MONSTER_ZOMBIE:
                    printf("▧");
                    break;
                case CASE_MONSTER_HARPY:
                    printf("▧");
                    break;
                case CASE_MONSTER_BASILIC:
                    printf("▧");
                    break;
                case CASE_MONSTER_TROLL:
                    printf("▧");
                    break;

                // player turn here for simplification
                case PLAYER_GREEN:
                    printf("▧");
                    break;
                case PLAYER_BLUE:
                    printf("▧");
                    break;
                case PLAYER_WHITE:
                    printf("▧");
                    break;
                case PLAYER_YELLOW:
                    printf("▧");
                    break;

                default:
                    // supposed that the only things here is case empty
                    platform_color_change(COLOR_EMPTY, COLOR_EMPTY);
                    printf("  ");
                    break;
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}

//  move the current player
void player_move(Board_Case **map, Case_Type turn, Choosen_Weapon weapon) {
}

void map_destroy(Board_Case **map) {
    // verify parameters
    if (map == NULL) {
        printf("Map is null on map_destroy\n");
        exit(2);
    }
    // we unalloc lines here
    for (u8 i = 0; i < MAP_SIZE; i++) {
        platform_free(map[i]);
    }
    platform_free(map);
}

// reveal the case
void reveal_case(Board_Case **map) {
    // verify parameters
    if (map == NULL) {
        printf("Map is null on map_destroy\n");
        exit(2);
    }
}

void empty_stdin_buffer() {
    while (getchar() != '\n') {
        // empty the stdin buffer until we found the next next line
    };
}