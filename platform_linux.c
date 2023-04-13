#include "./platform.h"

#ifdef PLATFORM_LINUX
#if _POSIX_C_SOURCE >= 199309L
#include <time.h> // nanosleep
#else
#include <unistd.h> // usleep
#endif

void platform_sleep(u64 ms) {
#if _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000 * 1000;
    nanosleep(&ts, 0);
#else
    if (ms >= 1000) {
        sleep(ms / 1000);
    }
    usleep((ms % 1000) * 1000);
#endif
}

void platform_srand() {
    /* Intializes random number generator */
    srand((unsigned)time(NULL));
}

void platform_console_clear() {
    printf("\033[H\033[J");
}
void platform_console_clean() {
    while (getchar() != '\n') { // clean input
    }
}

void platform_color_change_red() {
    printf("\033[31m");
}
void platform_color_change_blue() {
    printf("\033[34m");
}
void platform_color_change_reset() {
    printf("\033[0m");
}

void *platform_allocate(u64 size, b8 aligned) {
    void *r = malloc(size);
    if (r == NULL) {
        printf("malloc failed");
        exit(1);
    }
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