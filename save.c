#include "./save.h"
#include "./game.h"
#include "./platform.h"

b8 save_file_exists() {
    FILE *f = fopen(SAVE_FILE_NAME, "r");
    if (f == NULL) {
        return false;
    }
    fclose(f);
    return true;
}

// delete the save file after the party has ended
b8 remove_save() {
    return remove(SAVE_FILE_NAME);
}

b8 save_game(Board_Case **map, Case_Type turn, Choosen_Weapon weapon, u8 treasure_found, b8 artifact_found) {
    // create the file handle
    FILE *save_file = fopen(SAVE_FILE_NAME, "wb");
    if (save_file == NULL) {
        printf("Can't open file %s\nError code = %d\nAborting save.\n", SAVE_FILE_NAME, errno);
        // failed to save
        return false;
    }

#ifdef DEBUG
    printf("Write data to file...\n");
#endif

    // fill in the struct that we'll save
    Save_File_Struct file_struct;

    // write in the file
    fwrite(&file_struct, sizeof(Save_File_Struct), 1, save_file);

    // close stream
    fclose(save_file);

#ifdef DEBUG
    printf("Save success\n");
#endif

    // save success
    return true;
}

b8 load_game(Board_Case ***map, Case_Type *turn, Choosen_Weapon *weapon, u8 *treasure_found, b8 *artifact_found) {
    // create the file handle
    FILE *save_file = fopen(SAVE_FILE_NAME, "rb");
    if (save_file == NULL) {
        printf("Can't open file %s\nError code = %d\nAborting load.\n", SAVE_FILE_NAME, errno);
        // failed to load
        return false;
    }

#ifdef DEBUG
    printf("Read data form file...\n");
#endif

    // loading 1 struct from the file
    Save_File_Struct file_struct;
    fread(&file_struct, sizeof(Save_File_Struct), 1, save_file);

#ifdef DEBUG
    printf("Success\n");
    platform_sleep(5000);
#endif

    return true;
}