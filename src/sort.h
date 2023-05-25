#ifndef HEADER_SORT
#define HEADER_SORT 1

#include "defines.h"
#include "save.h"

typedef enum Sort_Type {
    SORT_TOP,
    SORT_KILL,
    SORT_TREASURE,
    SORT_WIN,

    // for errors
    SORT_UNKNOWN
} Sort_Type;

void swap(Save_Player_Score *a, Save_Player_Score *b);
i32 partition_top(Save_Player_Score array[], i32 low, i32 high);
i32 partition_kill(Save_Player_Score array[], i32 low, i32 high);
i32 partition_treasure(Save_Player_Score array[], i32 low, i32 high);
i32 partition_win(Save_Player_Score array[], i32 low, i32 high);
void quickSort(Save_Player_Score array[], i32 low, i32 high, Sort_Type type);

#endif