/*************************************************************************************
*    12 April 2021
*    COMP2017 Assignment 2 - VM
*    Owen Yang
*    500550430
*    DESCRIPTION: HEADER FILE FOR objdump.c
*************************************************************************************/

#ifndef OBJ
#define OBJ

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>

#define IS_BIT_SET(BF, N) ((BF >> N) & 0x1)

/*************************************************************************************
* ENUM DECLARATIONS.
*************************************************************************************/

enum bool {
    True = 1, 
    False = 0
};

/*************************************************************************************
* FUNCTION DECLARATIONS.
*************************************************************************************/

/*
* Power Function - Maths Library Reconstruct
*/
int power(int base, int pow);

/*
* Copies the Character Instruction into instruction array
*/
void copy_char(int new_inner_state, int* innerstate, int* count, int *arr_index, int *current_state_counter, int* inner_state_times);

/*
* The main function to return the instruction array in a struct
*/
struct ret objdump(char* argv);

#endif