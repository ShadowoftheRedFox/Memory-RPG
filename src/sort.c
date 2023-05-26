#include "sort.h"
#include <stdio.h>
#include <stdlib.h>

// function to swap elements
void swap(Save_Player_Score *a, Save_Player_Score *b) {
    // verify parameters
    if (a == NULL) {
        printf("a is null in swap\n");
        exit(1);
    }
    if (b == NULL) {
        printf("b is null in swap\n");
        exit(1);
    }

    Save_Player_Score t = *a;
    *a = *b;
    *b = t;
}

// function to find the partition position
i32 partition_top(Save_Player_Score array[], i32 low, i32 high) {
    // verify parameters
    if (array == NULL) {
        printf("array is null in partition_top\n");
        exit(1);
    }
    if (low < 0) {
        printf("low is out of range in partition_top\n");
        exit(1);
    }
    if (high < 0) {
        printf("high is out of range in partition_top\n");
        exit(1);
    }

    // select the rightmost element as pivot
    Save_Player_Score pivot = array[high];

    // pointer for greater element
    i32 i = (low - 1);

    // traverse each element of the array
    // compare them with the pivot
    for (int j = low; j < high; j++) {
        if (array[j].game_won * SCORE_WIN + array[j].monster_killed * SCORE_KILL + array[j].treasure_found * SCORE_TREASURE >=
            pivot.game_won * SCORE_WIN + pivot.monster_killed * SCORE_KILL + pivot.treasure_found * SCORE_TREASURE) {

            // if element smaller than pivot is found
            // swap it with the greater element pointed by i
            i++;

            // swap element at i with element at j
            swap(&array[i], &array[j]);
        }
    }

    // swap the pivot element with the greater element at i
    swap(&array[i + 1], &array[high]);

    // return the partition point
    return (i + 1);
}

i32 partition_kill(Save_Player_Score array[], i32 low, i32 high) {
    // verify parameters
    if (array == NULL) {
        printf("array is null in partition_kill\n");
        exit(1);
    }
    if (low < 0) {
        printf("low is out of range in partition_kill\n");
        exit(1);
    }
    if (high < 0) {
        printf("high is out of range in partition_kill\n");
        exit(1);
    }

    // select the rightmost element as pivot
    Save_Player_Score pivot = array[high];

    // pointer for greater element
    i32 i = (low - 1);

    // traverse each element of the array
    // compare them with the pivot
    for (int j = low; j < high; j++) {
        if (array[j].monster_killed >= pivot.monster_killed) {

            // if element smaller than pivot is found
            // swap it with the greater element pointed by i
            i++;

            // swap element at i with element at j
            swap(&array[i], &array[j]);
        }
    }

    // swap the pivot element with the greater element at i
    swap(&array[i + 1], &array[high]);

    // return the partition point
    return (i + 1);
}

i32 partition_treasure(Save_Player_Score array[], i32 low, i32 high) {
    // verify parameters
    if (array == NULL) {
        printf("array is null in partition_treasure\n");
        exit(1);
    }
    if (low < 0) {
        printf("low is out of range in partition_treasure\n");
        exit(1);
    }
    if (high < 0) {
        printf("high is out of range in partition_treasure\n");
        exit(1);
    }

    // select the rightmost element as pivot
    Save_Player_Score pivot = array[high];

    // pointer for greater element
    i32 i = (low - 1);

    // traverse each element of the array
    // compare them with the pivot
    for (int j = low; j < high; j++) {
        if (array[j].treasure_found >= pivot.treasure_found) {

            // if element smaller than pivot is found
            // swap it with the greater element pointed by i
            i++;

            // swap element at i with element at j
            swap(&array[i], &array[j]);
        }
    }

    // swap the pivot element with the greater element at i
    swap(&array[i + 1], &array[high]);

    // return the partition point
    return (i + 1);
}

i32 partition_win(Save_Player_Score array[], i32 low, i32 high) {
    // verify parameters
    if (array == NULL) {
        printf("array is null in partition_win\n");
        exit(1);
    }
    if (low < 0) {
        printf("low is out of range in partition_win\n");
        exit(1);
    }
    if (high < 0) {
        printf("high is out of range in partition_win\n");
        exit(1);
    }

    // select the rightmost element as pivot
    Save_Player_Score pivot = array[high];

    // pointer for greater element
    i32 i = (low - 1);

    // traverse each element of the array
    // compare them with the pivot
    for (int j = low; j < high; j++) {
        if (array[j].game_won >= pivot.game_won) {

            // if element smaller than pivot is found
            // swap it with the greater element pointed by i
            i++;

            // swap element at i with element at j
            swap(&array[i], &array[j]);
        }
    }

    // swap the pivot element with the greater element at i
    swap(&array[i + 1], &array[high]);

    // return the partition point
    return (i + 1);
}

void quickSort(Save_Player_Score array[], i32 low, i32 high, Sort_Type type) {
    // verify parameters
    if (array == NULL) {
        printf("array is null in quickSort\n");
        exit(1);
    }
    if (type < 0 || type >= SORT_UNKNOWN) {
        printf("type is out of range in quickSort\n");
        exit(1);
    }

    if (low < high) {
        // find the pivot element such as
        // elements smaller than pivot are on left of pivot
        // elements greater than pivot are on right of pivot
        i32 pi;
        if (type == SORT_TOP) {
            pi = partition_top(array, low, high);
        } else if (type == SORT_WIN) {
            pi = partition_win(array, low, high);
        } else if (type == SORT_TREASURE) {
            pi = partition_treasure(array, low, high);
        } else if (type == SORT_KILL) {
            pi = partition_kill(array, low, high);
        } else {
            printf("type is out of range\n");
            exit(1);
        }

        // recursive call on the left of pivot
        quickSort(array, low, pi - 1, type);

        // recursive call on the right of pivot
        quickSort(array, pi + 1, high, type);
    }
}