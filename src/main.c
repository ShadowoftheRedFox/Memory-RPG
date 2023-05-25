#include "./game.h"
#include "./menu.h"
#include "./platform.h"
#include "./save.h"

int main(int argc, char const *argv[]) {
    // setup game
    platform_srand();

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

    // TODO best score leaderboard?

    // runs menu
    while (menu_running) {
        if (actual_menu == MENU_MAIN) {
            // we are in the main menu
            printf("MENU\n1 - LOAD GAME\n2 - NEW GAME\n3 - SCORES\n4 - QUIT\n");
            do {
                printf(">> ");
                correct = scanf("%d", &menu_choice);
            } while (menu_choice < 1 || menu_choice > 4 || correct != 1);
            switch (menu_choice) {
            case 1: // load actual game
                // check if there is a save
                if (!save_file_exists(SAVE_FOLDER SAVE_FILE_NAME)) {
                    printf("There is no game to resume.\n");
                    actual_menu = MENU_MAIN;
                } else {
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
                printf("Goodbye! See you again!\n");
                break;
            }
        } else if (actual_menu == MENU_GAME) {
            // runs the game for the round
            while (game_running) {
                // [turn - PLAYER_BLUE] gives the correct number in the array depending
                // of the turn, whatever is the number of player
                turn_number = turn - PLAYER_BLUE;
                // count the round for each players
                round_number[turn_number]++;
                save_game(map, player_number, treasure_found, monster_killed,
                          round_number, treasure, will_teleport, artifact_found,
                          player_x, player_y, active_weapon, player_class, player_name,
                          turn, is_winner);

                if (is_winner) {
                    game_win(turn, player_name[turn_number], round_number[turn_number]);
                    save_score(player_name[turn_number], treasure_found[turn_number],
                               monster_killed[turn_number], 1);
                    u32 answer;
                    printf("Do you want to play a new game?\n1 - Yes\n2 - No\n");
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
                        printf("Readying up to make a new game...\n");
                        map_setup(map);
                        reset_variables(&player_number, treasure_found, monster_killed,
                                        round_number, treasure, will_teleport,
                                        artifact_found, player_x, player_y, &active_weapon,
                                        player_class, player_name, &turn, &is_winner);
                        // launch the new game
                        new_game(&player_number, player_name, player_class);
                        // save the score of each player
                        for (u8 hihi = 0; hihi < player_number; hihi++) {
                            save_score(player_name[hihi], 0, 0, 0);
                        }
                        actual_menu = MENU_GAME;
                        game_running = true;
                    }
                }

                // say whose turn is who, with the right color
                printf("It's ");
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
                printf("%s", player_name[turn_number]);
                platform_color_change(COLOR_EMPTY, COLOR_EMPTY);
                printf(" turn!\n\n");

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
                    player_teleport(map, &turn, &player_x[turn_number],
                                    &player_y[turn_number]);
                }

                // logic after the move
                game_logic(map, &turn, &treasure[turn_number],
                           &monster_killed[turn_number], &artifact_found[turn_number],
                           player_class[turn_number], active_weapon, player_number,
                           &will_teleport[turn_number], &player_x[turn_number],
                           &player_y[turn_number], &is_winner);
            }
        }
    }

    map_destroy(map);
    return 0;
}