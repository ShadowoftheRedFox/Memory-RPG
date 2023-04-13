#include "./game.h"
#include "./platform.h"

Board_Case **map_create() {
    // here malloc(size); = platform_allocate(size, false);
    // we alloc columns here
    Board_Case **map = platform_allocate(MAP_SIZE * sizeof(Board_Case *), false);
    // we test if malloc failed
    if (map == NULL) {
        printf("malloc failed.\nError: %d\n", errno);
        exit(1);
    }

    // we alloc lines here
    for (u8 i = 0; i < MAP_SIZE; i++) {
        map[i] = platform_allocate(MAP_SIZE * sizeof(Board_Case), false);
        if (map[i] == NULL) {
            printf("malloc failed.\nError: %d\n", errno);
            exit(1);
        }
    }

    // fill the map
    for (u8 y = 0; y < MAP_SIZE; y++) {
        for (u8 x = 0; x < MAP_SIZE; x++) {
            // our current case is map[y][x]
            // TODO add player spawn
            if (y == 0 || x == 0 || x == MAP_SIZE - 1 || y == MAP_SIZE - 1) {
                // case that are empty
                map[y][x].empty = true;
            } else {
                // case hidden
                // TODO fill content
                map[y][x].empty = false;
                map[x][y].hidden = true;
            }
        }
    }

    return map;
}