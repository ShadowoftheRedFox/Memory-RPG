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

void *platform_allocate(u64 size, b8 aligned) {
    void *r = malloc(size);
    if (r == NULL) {
        printf("malloc failed");
        exit(1);
    }
    return r;
}

void platform_free(void *block, b8 aligned) {
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