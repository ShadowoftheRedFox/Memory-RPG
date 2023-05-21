#ifndef HEADER_PLATFORM
#define HEADER_PLATFORM 1

#if defined(__linux__) || defined(__gnu_linux__)
#include <errno.h>
#endif

#include "./defines.h"
#include "./game.h"
#include <stdio.h>
#include <stdlib.h>

void platform_console_clear();
// remove remaining char in the stdin buffer
void platform_console_clean();

void platform_color_change_red();
void platform_color_change_blue();
void platform_color_change_reset();

void *platform_allocate(u64 size);
void platform_free(void *block);
void *platform_zero_memory(void *block, u64 size);
void *platform_copy_memory(void *dest, const void *source, u64 size);
void *platform_set_memory(void *dest, i32 value, u64 size);
void platform_color_change(Case_Color font, Case_Color back);

void platform_sleep(u64 ms);

void platform_srand();
#endif