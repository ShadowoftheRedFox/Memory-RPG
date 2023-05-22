#include "./platform.h"

#ifdef PLATFORM_WINDOWS
#include <conio.h>
#include <time.h>
#include <windows.h>

void platform_sleep(u64 ms) {
    Sleep(ms);
}

void platform_srand() {
    /* Intializes random number generator */
    srand((unsigned)time(NULL));
}

void platform_console_clear() {
    system("cls");
}

void platform_console_clean() {
    while (getchar() != '\n') { // clean input
    }
}

void platform_color_change_red() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
}
void platform_color_change_blue() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
}
void platform_color_change_reset() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void platform_color_change(Case_Color font, Case_Color back) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD attributes = 0;

    // set background color
    switch (back) {
    case COLOR_RED:
        attributes = attributes | BACKGROUND_RED | BACKGROUND_INTENSITY;
        break;
    case COLOR_GREEN:
        attributes = attributes | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
        break;
    case COLOR_BLUE:
        attributes = attributes | BACKGROUND_BLUE;
        break;
    case COLOR_PINK:
        attributes = attributes | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY;
        break;
    case COLOR_YELLOW:
        attributes = attributes | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
        break;
    case COLOR_WHITE:
        attributes = attributes | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
        break;
    case COLOR_EMPTY:
        // Specify this one to just pass color, and not make a case for empty cases, black background
        attributes = 0;
        break;
    default:
        // it's all existing color
        attributes = 0;
        break;
    }

    // set font color
    switch (font) {
    case COLOR_RED:
        attributes = attributes | FOREGROUND_RED | FOREGROUND_INTENSITY;
        break;
    case COLOR_GREEN:
        attributes = attributes | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        break;
    case COLOR_BLUE:
        attributes = attributes | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;
    case COLOR_YELLOW:
        attributes = attributes | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        break;
    case COLOR_PINK:
        attributes = attributes | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
        break;
    case COLOR_WHITE:
        attributes = attributes | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN;
        break;
    case COLOR_EMPTY:
        // Specify this one to just pass color, and not make a case for empty cases, white font
        if (back == COLOR_EMPTY) {
            attributes = 0;
        }
        break;
    default:
        // it's all existing color
        attributes = 0;
        break;
    }

    // change colors
    SetConsoleTextAttribute(hConsole, attributes);
}
void *platform_allocate(u64 size) {
    void *r = malloc(size);
    if (r == NULL) {
        printf("malloc failed");
        exit(1);
    }
    return r;
}

void platform_free(void *block) {
    free(block);
}

void *platform_zero_memory(void *block, u64 size) {
    return memset(block, 0, size);
}

void *platform_copy_memory(void *dest, const void *source, u64 size) {
    return memcpy(dest, source, size);
}

void *platform_set_memory(void *dest, i32 value, u64 size) {
    return memset(dest, value, size);
}
#endif