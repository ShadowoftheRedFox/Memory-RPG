#include "./game.h"
#include "./menu.h"
#include "./platform.h"
#include "./save.h"
#include <time.h>

int main(int argc, char const *argv[]) {
    // setup game rand
    platform_srand();
    platform_console_clear();

    // create the map
    Board_Case **map = map_create();

    // setup variables
    b8 game_running = true;
    b8 menu_running = true;
    Menu_Type actual_menu = MENU_MAIN;
    u8 confirm = 0;
    i32 correct = 0;
    u8 turn_number = 0;
    u32 menu_choice = 0;
    u32 answer = 0;

    // for counting the score
    u32 player_number = 0;
    u32 treasure_found[MAX_PLAYER] = {0, 0, 0, 0};
    u32 monster_killed[MAX_PLAYER] = {0, 0, 0, 0};

    // for the current round
    u32 round_number[MAX_PLAYER] = {0, 0, 0, 0};
    u32 treasure[MAX_PLAYER] = {0, 0, 0, 0};
    b8 will_teleport[MAX_PLAYER] = {false, false, false, false};
    b8 artifact_found[MAX_PLAYER] = {false, false, false, false};
    u32 player_x[MAX_PLAYER] = {0, 0, 0, 0};
    u32 player_y[MAX_PLAYER] = {0, 0, 0, 0};
    Choosen_Weapon active_weapon = WEAPON_UNKNOWN;

    // for the current game
    Class_Type player_class[MAX_PLAYER] = {CLASS_UNKNOWN, CLASS_UNKNOWN, CLASS_UNKNOWN, CLASS_UNKNOWN};
    char player_name[MAX_PLAYER][PLAYER_NAME_LENGTH];
    Case_Type turn = PLAYER_BLUE;
    b8 is_winner = false;
    // to know the unique number of the save
    time_t save_id;

    // setup the coordinates for the start
    // player blue
    player_x[0] = 0;
    player_y[0] = 2;
    // player green
    player_x[1] = 4;
    player_y[1] = 0;
    // player white
    player_x[2] = MAP_SIZE - 1;
    player_y[2] = 4;
    // player yellow
    player_x[3] = 2;
    player_y[3] = MAP_SIZE - 1;

    // runs menu
    while (menu_running) {
        if (actual_menu == MENU_MAIN) {
            // we are in the main menu
            platform_console_clear();
            animate_printf("Menu:\n1 - Load game\n2 - New game\n3 - Scores\n4 - Quit game\n");
            do {
                printf(">> ");
                correct = scanf("%d", &menu_choice);
            } while (menu_choice < 1 || menu_choice > 4 || correct != 1);
            platform_console_clear();
            switch (menu_choice) {
            case 1: // load actual game
                // check if there is a save
                if (!save_file_exists(SAVE_FOLDER SAVE_FILE_NAME)) {
                    animate_printf("There is no game to resume.\n");
                    platform_sleep(1000);
                    actual_menu = MENU_MAIN;
                } else {
                    animate_printf("Loading your game...");
                    platform_sleep(750);
                    load_game(map, &player_number, treasure_found, monster_killed,
                              round_number, treasure, will_teleport, artifact_found,
                              player_x, player_y, &active_weapon, player_class,
                              player_name, &turn, &is_winner);
                    game_running = true;
                    actual_menu = MENU_GAME;
                }
                break;
            case 2: // new game
                map_setup(map);
                reset_variables(&player_number, treasure_found, monster_killed,
                                round_number, treasure, will_teleport, artifact_found,
                                player_x, player_y, &active_weapon, player_class,
                                player_name, &turn, &is_winner);
                // launch the new game
                new_game(&player_number, player_name, player_class);
                // get the current time in second
                save_id = time(NULL);
                // save the score of each player
                for (u8 hihi = 0; hihi < player_number; hihi++) {
                    save_score(player_name[hihi], 0, 0, 0);
                }
                actual_menu = MENU_GAME;
                game_running = true;
                break;
            case 3: // score
                score_menu();
                break;
            case 4: // quit
                menu_running = false;
                animate_printf("Goodbye! See you again!\n");
                platform_sleep(2000);
                break;
            }
        } else if (actual_menu == MENU_GAME) {
            // runs the game for the round
            while (game_running) {
                platform_console_clear();
                // [turn - PLAYER_BLUE] gives the correct number in the array depending
                // of the turn, whatever is the number of player
                turn_number = turn - PLAYER_BLUE;
                // count the round for each players
                round_number[turn_number]++;

                if (is_winner) {
                    // display win message
                    game_win(turn, player_name[turn_number], round_number[turn_number]);
                    // save the score of each player
                    for (u8 i = 0; i < player_number; i++) {
                        save_score(player_name[i], treasure_found[i], monster_killed[i],
                                   // add the win to the player who won
                                   (i == turn_number) ? 1 : 0);
                    }
                    platform_console_clear();
                    animate_printf("Do you want to play a new game?\n1 - Yes\n2 - No\n");
                    do {
                        printf(">> ");
                        correct = scanf("%d", &answer);
                        empty_stdin_buffer();
                    } while (correct != 1 || answer < 1 || answer > 2);

                    if (answer == 2) {
                        game_running = false;
                        menu_running = true;
                        actual_menu = MENU_MAIN;
                        break;
                    } else {
                        platform_console_clear();
                        animate_printf("Readying up to make a new game...\n");
                        platform_sleep(750);
                        map_setup(map);
                        reset_variables(&player_number, treasure_found, monster_killed,
                                        round_number, treasure, will_teleport,
                                        artifact_found, player_x, player_y, &active_weapon,
                                        player_class, player_name, &turn, &is_winner);
                        // launch the new game
                        new_game(&player_number, player_name, player_class);
                        // get the current time in second
                        save_id = time(NULL);
                        actual_menu = MENU_GAME;
                        game_running = true;
                    }
                }

                // say whose turn is who, with the right color
                animate_printf("It's ");
                switch (turn) {
                case PLAYER_BLUE:
                    platform_color_change(COLOR_BLUE, COLOR_EMPTY);
                    break;
                case PLAYER_GREEN:
                    platform_color_change(COLOR_GREEN, COLOR_EMPTY);
                    break;
                case PLAYER_YELLOW:
                    platform_color_change(COLOR_YELLOW, COLOR_EMPTY);
                    break;
                case PLAYER_WHITE:
                    platform_color_change(COLOR_EMPTY, COLOR_WHITE);
                    break;
                default:
                    platform_color_change(COLOR_EMPTY, COLOR_EMPTY);
                    break;
                }
                animate_printf(player_name[turn_number]);
                platform_color_change(COLOR_EMPTY, COLOR_EMPTY);
                animate_printf(" turn!\n\n");

                //  show the board
                map_print(map);

                // ask for which weapon for this round
                game_choose_weapon(&active_weapon);

                // await player move
                if (!will_teleport[turn_number]) {
                    player_move(map, &turn, player_number, &player_x[turn_number],
                                &player_y[turn_number], &treasure[turn_number],
                                &artifact_found[turn_number]);
                } else {
                    will_teleport[turn_number] = false;
                    player_teleport(map, &player_x[turn_number], &player_y[turn_number]);
                }

                // logic after the move
                game_logic(map, &turn, &treasure[turn_number],
                           &monster_killed[turn_number], &artifact_found[turn_number],
                           player_class[turn_number], active_weapon, player_number,
                           &will_teleport[turn_number], &player_x[turn_number],
                           &player_y[turn_number], &is_winner);

                save_game(map, player_number, treasure_found, monster_killed,
                          round_number, treasure, will_teleport, artifact_found,
                          player_x, player_y, active_weapon, player_class, player_name,
                          turn, is_winner);
            }
        }
    }

    map_destroy(map);
    return 0;
}