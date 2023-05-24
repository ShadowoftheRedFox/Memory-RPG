#if defined(__linux__) || defined(__gnu_linux__)
#include "./platform.h"
#include <errno.h>
#include <string.h>

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

void platform_console_clear() { printf("\033[H\033[J"); }
void platform_console_clean() {
  while (getchar() != '\n') { // clean input
  }
}

void platform_color_change(Case_Color font, Case_Color background) {
  u32 back = 0;
  // back color
  switch (background) {
  case COLOR_RED:
    back = 41;
    break;
  case COLOR_GREEN:
    back = 42;
    break;
  case COLOR_BLUE:
    back = 44;
    break;
  case COLOR_YELLOW:
    back = 43;
    break;
  case COLOR_PINK:
    back = 45;
    break;
  case COLOR_WHITE:
    back = 47;
    break;
  case COLOR_EMPTY:
  default:
    // Specify this one to just pass color, and not make a case for empty cases
    switch (font) {
    case COLOR_RED:
      printf("\033[31m");
      break;
    case COLOR_GREEN:
      printf("\033[32m");
      break;
    case COLOR_BLUE:
      printf("\033[34m");
      break;
    case COLOR_YELLOW:
      printf("\033[33m");
      break;
    case COLOR_PINK:
      printf("\033[35m");
      break;
    case COLOR_WHITE:
      printf("\033[37m");
      break;
    default:
    case COLOR_EMPTY:
      // Specify this one to just pass color, and not make a case for empty
      // cases
      printf("\033[0m");
      break;
    }
    return;
    break;
  }

  // font color
  switch (font) {
  case COLOR_RED:
    printf("\033[31;%dm", back);
    break;
  case COLOR_GREEN:
    printf("\033[32;%dm", back);
    break;
  case COLOR_BLUE:
    printf("\033[34;%dm", back);
    break;
  case COLOR_YELLOW:
    printf("\033[33;%dm", back);
    break;
  case COLOR_PINK:
    printf("\033[35;%dm", back);
    break;
  case COLOR_WHITE:
    printf("\033[37;%dm", back);
    break;
  case COLOR_EMPTY:
    // Specify this one to just pass color, and not make a case for empty cases
    printf("\033[0m");
    break;
  default:
    // it's all existing color
    break;
  }
}

void *platform_allocate(u64 size) {
  void *r = malloc(size);
  if (r == NULL) {
    printf("malloc failed");
    exit(1);
  }
}
void platform_free(void *block) { free(block); }
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