#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>

#define IS_BIT_SET(BF, N) ((BF >> N) & 0x1)

enum bool {
    True = 1, 
    False = 0
};

int power(int base, int pow);

void copy_char(int new_inner_state, int* innerstate, int* count, int *arr_index, int *current_state_counter, int* inner_state_times);

struct ret objdump(char* argv);
