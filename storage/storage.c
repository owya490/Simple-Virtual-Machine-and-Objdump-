#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>

#define IS_BIT_SET(BF, N) ((BF >> N) & 0x1)

typedef enum { // v = value; x = register; s = Stack, p = pointer
    MOV = 'm', 
    CAL = 'c',
    RET = 'R',
    REF = 'r', 
    ADD = 'a',
    PRINT = 'o', // can clash with Stack names A- Z
    NOT = 'n',
    EQU = 'e'
}instr;

typedef enum {
    VAL, 
    REG, 
    STK, 
    PTR
}vtype;

enum bool {
    True = 1, 
    False = 0
};

struct operand{
    instr op;
    vtype vtype1;
    int val1;
    vtype vtype2;
    int val2;
};


int power(int base, int pow) {
    if (pow == 0) {
        return 1;
    }
    int ret = base;
    for (int i = 1; i < pow; i ++) {
        ret *= base;
    }
    return ret;
}

void copy_char(int new_inner_state, int* innerstate, int* count, int *arr_index, int *current_state_counter, int* inner_state_times) {
    *innerstate = new_inner_state;
    *count = -1;
    *arr_index+=1;
    *current_state_counter = 0;
    *inner_state_times +=1;
}


int main(int argc, char** argv) {
    FILE* f = fopen(argv[1], "rb");
    struct stat stats;
    stat(argv[1], &stats);
    //tests/sample_program.x2017
    int size = stats.st_size;
    //printf("size: %d\n", size);

    uint8_t* buffer = (uint8_t *) malloc(sizeof(uint8_t) * size);

    int start = True;
    int count = 0;
    int total_count = 0;

    int method_count = 0; // total number of method counts;
    int method_count_times = 0; //how many methods have elapsed
    //int end_of_method = False; 
    int in_method = False; //if we are curernlty in the middle of translating a method

    int is_padding = False; // show that the loop is just reading padding now

    int state = 0; // which operand state it is in
    int inner_state = 0; // which state in operand read it is in
    int innner_state_times = 0; // how many times inner state has elapsed e.g. add needs 4 times 
    int current_state_counter = 0; //mainly a binary to dec counter

    unsigned char* arr = (unsigned char*) malloc(1);
    int arr_size = 0;
    int arr_index = 0;

    // int symbol[32];
    // for (int u = 0; u < 32; u ++) {
    //     symbol[u] = 33;
    // }
    // int symbol_index = 0;

    // int number_symbols[8];
    // int number_symbols_index = 0;
    // int function_number_symbols = 0;



    fread(buffer, 1, size, f);

    for (int i = 0; i < size; i ++) {
        for (int j = 0; j < 8; j ++) {
            //(IS_BIT_SET(buffer[size - i - 1], j)) ? printf("1") : printf("0");

            if (is_padding) {
            }

            else if (start && count < 5) {
                if (IS_BIT_SET(buffer[size - i - 1], j)) {method_count += power(2, count);}
                //(IS_BIT_SET(buffer[size - i - 1], j)) ? printf("1") : printf("0");
                if (count == 4) {
                    start = False;
                    arr_size++;
                    arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                    arr[arr_index] = 'R';
                    arr_index ++;
                    count = -4;
                    method_count_times ++;
                    state = 0;
                }
            }

            else if (method_count_times == method_count && !in_method) {
                if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                if (count == 2) {
                    arr_size += 2;
                    arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                    arr[arr_index] = current_state_counter + '0';
                    arr_index++;
                    arr[arr_index] = '|';
                    arr_index ++;
                    start = True;
                    state = 0;
                    inner_state = 0;
                    innner_state_times = 0;
                    current_state_counter = 0;
                    // for (int u = 0; u < 32; u ++) {
                    //     symbol[u] = 33;
                    // }
                    // symbol_index = 0;
                    // number_symbols[number_symbols_index] = function_number_symbols;
                    // number_symbols_index++;
                    // //printf("%d\n", number_symbols[0]);
                    // function_number_symbols = 0;
                    count = -1;
                    method_count = 0;
                    method_count_times = 0;
                    if ((size * 8) - total_count <= 8) {
                        is_padding = True;
                    }
                }

            }

            else if (state == 0 && count < 3 && count >= 0) {
                if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                if (count == 2) {
                    method_count_times ++;
                    if (current_state_counter == 0) { //MOV
                        state = 1;
                        arr_size += 5;
                        arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                        arr[arr_index] = 'm';
                        arr_index ++;
                        count = -1;
                        current_state_counter = 0;
                        in_method = True;
                    }

                    else if (current_state_counter == 1) { //CAL
                        state = 2;
                        arr_size += 3;
                        arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                        arr[arr_index] = 'c';
                        arr_index++;
                        count = -1;
                        current_state_counter = 0;
                        in_method = True;
                    }

                    else if (current_state_counter == 2) { //RET
                        //printf("HERE");
                        state = 3;
                        arr_size += 1;
                        arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                        arr[arr_index] = 'R';
                        arr_index++;
                        count = -1;
                        current_state_counter = 0;
                        state = 0;
                        inner_state = 0;
                        innner_state_times = 0;
                        current_state_counter  = 0;
                        in_method = True;
                        in_method = False;

                    }

                    else if (current_state_counter == 3) { //REF
                        state = 4;
                        arr_size += 5;
                        arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                        arr[arr_index] = 'r';
                        arr_index++;
                        count = -1;
                        current_state_counter = 0;
                        in_method = True;
                    }

                    else if (current_state_counter == 4) { //ADD
                        state = 5;
                        arr_size += 5;
                        arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                        arr[arr_index] = 'a';
                        arr_index ++;
                        count = -1;
                        current_state_counter = 0;
                        in_method = True;
                    }

                    else if (current_state_counter == 5) { //PRINT
                        state = 6;
                        arr_size += 3;
                        arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                        arr[arr_index] = 'o';
                        arr_index++;
                        count = -1;
                        current_state_counter = 0;
                        in_method = True;
                    }

                    else if (current_state_counter == 6) { //NOT
                        state = 7;
                        arr_size += 3;
                        arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                        arr[arr_index] = 'n';
                        arr_index++;
                        count = -1;
                        current_state_counter = 0;
                        in_method = True;
                    }

                    else if (current_state_counter == 7) { //EQU
                        state = 8;
                        arr_size += 3;
                        arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                        arr[arr_index] = 'e';
                        arr_index++;
                        count = -1;
                        current_state_counter = 0;
                        in_method = True;
                    }
                }
            }

            else if (state == 1) {
                if (inner_state == 0) { // reading 2 bits, to see if stack or reg or val
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                    if (count == 1) {
                        if (current_state_counter == 0) { // value 
                            arr[arr_index] = 'v';
                            copy_char(1, &inner_state, &count, &arr_index, &current_state_counter, &innner_state_times);
                        }
                        else if (current_state_counter == 1) { // reg address
                            arr[arr_index] = 'x';
                            copy_char(2, &inner_state, &count, &arr_index, &current_state_counter, &innner_state_times);
                        }
                        else if (current_state_counter == 2) { // Stack symbol
                            arr[arr_index] = 's';
                            copy_char(3, &inner_state, &count, &arr_index, &current_state_counter, &innner_state_times);
                            
                        }
                        else if (current_state_counter == 3) { // Pointer
                            arr[arr_index] = 'p';
                            copy_char(4, &inner_state, &count, &arr_index, &current_state_counter, &innner_state_times);
                        }
                    }
                }

                else if (inner_state == 1) { // value
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                    if (count == 7) {
                        count = -1;
                        int current_state_counter_temp = current_state_counter;
                        int digits = 0;
                        while (current_state_counter_temp > 0) {
                            digits++;
                            current_state_counter_temp /= 10;
                        }
                        int* tmp = (int*) malloc(sizeof(int) * digits); // check if this works later when you get to value
                        arr_size++;
                        arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                        if (digits == 2) {
                            arr_size += 1;
                            arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                        } 
                        if (digits == 3) {
                            arr_size += 2;
                            arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                        }
                        int index_temp = 0;
                        while (current_state_counter > 0) {
                            int digit = current_state_counter % 10;
                            tmp[index_temp] = digit;
                            index_temp++;
                            current_state_counter /= 10;
                        }
                        arr[arr_index] = digits +'0';
                        arr_index ++;
                        for (int l = 0; l < digits; l ++) {
                            arr[arr_index] = tmp[digits - l - 1] + '0';
                            arr_index ++;
                        }

                        free(tmp);
                        current_state_counter = 0;
                        innner_state_times ++;
                        if (innner_state_times == 4) {
                            state = 0; //0
                            inner_state = 0;
                            innner_state_times = 0;
                            in_method = False;
                        } else {
                            inner_state = 0; //0
                        }
                    }
                }

                else if (inner_state == 2) { // reg
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                    if (count == 2) {
                        count = -1;
                        arr[arr_index] = current_state_counter + '0';
                        arr_index ++;
                        current_state_counter = 0;
                        innner_state_times ++;
                        if (innner_state_times == 4) {
                            state = 0; //0
                            inner_state = 0;
                            innner_state_times = 0;
                            in_method = False;
                        } else {
                            inner_state = 0; //0
                        }
                    }
                }

                else if (inner_state == 3) { // reading 5 bits for STACK ref
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                    if (count == 4) {
                        count = -1;
                        if (current_state_counter < 26) {
                            arr[arr_index] = current_state_counter + 65;
                        } else if (current_state_counter >= 26) {
                            arr[arr_index] = current_state_counter - 26 + 97;
                        }
                        // int checkSymbolExist = 0;
                        // for (int m = 0; m < 32; m++) {
                        //     if (symbol[m] == current_state_counter) {
                        //         if (m < 26) {
                        //             arr[arr_index] = m + 65;
                        //             checkSymbolExist = 1;
                        //         } else if (m >= 26) {
                        //             arr[arr_index] = m - 26 + 97;
                        //             checkSymbolExist = 1;
                        //         }
                        //     }
                        // }
                        // if (!checkSymbolExist) {
                        //     printf("hey");
                        //     symbol[symbol_index] = current_state_counter;
                        //     function_number_symbols ++;
                        //     if (symbol_index < 26) {
                        //         arr[arr_index] = symbol_index + 65;
                        //     } else if (current_state_counter >= 26) {
                        //         arr[arr_index] = symbol_index - 26 + 97;
                        //     }
                        //     symbol_index++;

                        // }
                        arr_index ++;
                        current_state_counter = 0;
                        innner_state_times ++;
                        if (innner_state_times == 4) {
                            state = 0; //0
                            inner_state = 0;
                            innner_state_times = 0;
                            in_method = False;
                        } else {
                            inner_state = 0; //0
                        }
                    }
                }

                else if (inner_state == 4) { // 5 bit for pointer
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                    if (count == 4) {
                        count = -1;
                        if (current_state_counter < 26) {
                            arr[arr_index] = current_state_counter + 65;
                        } else if (current_state_counter >= 26) {
                            arr[arr_index] = current_state_counter - 26 + 97;
                        }
                        // for (int m = 0; m < 32; m++) {
                        //     if (symbol[m] == current_state_counter) {
                        //         if (m < 26) {
                        //             arr[arr_index] = m + 65;
                        //         } else if (m >= 26) {
                        //             arr[arr_index] = m - 26 + 97;
                        //         }
                        //     }
                        // }
                        arr_index ++;
                        current_state_counter = 0;
                        innner_state_times ++;
                        if (innner_state_times == 4) {
                            state = 0; //0
                            inner_state = 0;
                            innner_state_times = 0;
                            in_method = False;
                        } else {
                            inner_state = 0; //0
                        }
                    }
                }
            }

            else if (state == 2) {
                if (inner_state == 0) {
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}

                    if (count == 1) {
                        if (current_state_counter == 0) { //value
                            arr[arr_index] = 'v';
                            copy_char(1, &inner_state, &count, &arr_index, &current_state_counter, &innner_state_times);
                        }
                    }
                }
                
                else if (inner_state == 1) {
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                    if (count == 7) {
                        count = -1;
                        int current_state_counter_temp = current_state_counter;
                        int digits = 0;
                        while (current_state_counter_temp > 0) {
                            digits++;
                            current_state_counter_temp /= 10;
                        }
                        int* tmp = (int*) malloc(sizeof(int) * digits); // check if this works later when you get to value
                        arr_size++;
                        arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                        if (digits == 2) {
                            arr_size += 1;
                            arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                        } 
                        if (digits == 3) {
                            arr_size += 2;
                            arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                        }
                        int index_temp = 0;
                        while (current_state_counter > 0) {
                            int digit = current_state_counter % 10;
                            tmp[index_temp] = digit;
                            index_temp++;
                            current_state_counter /= 10;
                        }
                        arr[arr_index] = digits +'0';
                        arr_index ++;
                        for (int l = 0; l < digits; l ++) {
                            arr[arr_index] = tmp[digits - l - 1] + '0';
                            arr_index ++;
                        }
                        free(tmp);
                        current_state_counter = 0;
                        innner_state_times ++;
                        if (innner_state_times == 2) {
                            state = 0; //0
                            inner_state = 0;
                            innner_state_times = 0;
                            in_method = False;
                        } else {
                            inner_state = 0; //0
                        }
                    }
                }
            }

            else if (state == 3) {
                //printf("|");
                //if (count == 2) {
                    //printf("here");
                state = 0; //0
                inner_state = 0;
                innner_state_times = 0;
                in_method = False;
                count= -1;
                current_state_counter = 0;
                //}
            }

            else if (state == 4) { //REF
                if (inner_state == 0) {
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                    if (count == 1) {    
                        if (current_state_counter == 1) { // reg address
                            arr[arr_index] = 'x';
                            copy_char(2, &inner_state, &count, &arr_index, &current_state_counter, &innner_state_times);
                        }
                        else if (current_state_counter == 2) { // Stack symbol
                            arr[arr_index] = 's';
                            copy_char(3, &inner_state, &count, &arr_index, &current_state_counter, &innner_state_times);
                        }
                        else if (current_state_counter == 3) { // Pointer
                            arr[arr_index] = 'p';
                            copy_char(4, &inner_state, &count, &arr_index, &current_state_counter, &innner_state_times);
                        }
                    }
                }
                else if (inner_state == 2) { // reg
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                    if (count == 2) {
                        count = -1;
                        arr[arr_index] = current_state_counter + '0';
                        arr_index ++;
                        current_state_counter = 0;
                        innner_state_times ++;
                        if (innner_state_times == 4) {
                            state = 0; //0
                            inner_state = 0;
                            innner_state_times = 0;
                            in_method = False;
                        } else {
                            inner_state = 0; //0
                        }
                    }
                }
                else if (inner_state == 3) {
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                    if (count == 4) {
                        count = -1;
                        if (current_state_counter < 26) {
                            arr[arr_index] = current_state_counter + 65;
                        } else if (current_state_counter >= 26) {
                            arr[arr_index] = current_state_counter - 26 + 97;
                        }
                        // for (int m = 0; m < 32; m++) {
                        //     if (symbol[m] == current_state_counter) {
                        //         if (m < 26) {
                        //             arr[arr_index] = m + 65;
                        //         } else if (m >= 26) {
                        //             arr[arr_index] = m - 26 + 97;
                        //         }
                        //     }
                        // }
                        arr_index ++;
                        current_state_counter = 0;
                        innner_state_times ++;
                        if (innner_state_times == 4) {
                            state = 0; //0
                            inner_state = 0;
                            innner_state_times = 0;
                            in_method = False;
                        } else {
                            inner_state = 0; //0
                        }
                    }
                }
                else if (inner_state == 4) {
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                    if (count == 4) {
                        count = -1;
                        if (current_state_counter < 26) {
                            arr[arr_index] = current_state_counter + 65;
                        } else if (current_state_counter >= 26) {
                            arr[arr_index] = current_state_counter - 26 + 97;
                        }
                        // for (int m = 0; m < 32; m++) {
                        //     if (symbol[m] == current_state_counter) {
                        //         if (m < 26) {
                        //             arr[arr_index] = m + 65;
                        //         } else if (m >= 26) {
                        //             arr[arr_index] = m - 26 + 97;
                        //         }
                        //     }
                        // }
                        arr_index ++;
                        current_state_counter = 0;
                        innner_state_times ++;
                        if (innner_state_times == 4) {
                            state = 0; //0
                            inner_state = 0;
                            innner_state_times = 0;
                            in_method = False;
                        } else {
                            inner_state = 0; //0
                        }
                    }
                }
            }

            else if (state == 5) {
                if (inner_state == 0) {
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}

                    if (count == 1) {
                        if (current_state_counter == 1) { // reg address
                            arr[arr_index] = 'x';
                            copy_char(2, &inner_state, &count, &arr_index, &current_state_counter, &innner_state_times);
                        }
                    }
                }

                else if (inner_state == 2) { // reg
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                    if (count == 2) {
                        count = -1;
                        arr[arr_index] = current_state_counter + '0';
                        arr_index ++;
                        current_state_counter = 0;
                        innner_state_times ++;
                        if (innner_state_times == 4) {
                            state = 0; //0
                            inner_state = 0;
                            innner_state_times = 0;
                            in_method = False;
                        } else {
                            inner_state = 0; //0
                        }
                    }
                }
            }

            else if (state == 6) {
                //printf("?");
                if (inner_state == 0) {
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}

                    if (count == 1) {
                        if (current_state_counter == 0) { //value
                            arr[arr_index] = 'v';
                            copy_char(1, &inner_state, &count, &arr_index, &current_state_counter, &innner_state_times);
                        }
                        else if (current_state_counter == 1) { // reg address
                            arr[arr_index] = 'x';
                            copy_char(2, &inner_state, &count, &arr_index, &current_state_counter, &innner_state_times);
                        }
                        else if (current_state_counter == 2) { // Stack symbol
                            arr[arr_index] = 's';
                            copy_char(3, &inner_state, &count, &arr_index, &current_state_counter, &innner_state_times);
                        }
                        else if (current_state_counter == 3) { // Pointer
                            arr[arr_index] = 'p';
                            copy_char(4, &inner_state, &count, &arr_index, &current_state_counter, &innner_state_times);
                        }
                    }
                }

                else if (inner_state == 1) {
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                    if (count == 7) {
                        count = -1;
                        int current_state_counter_temp = current_state_counter;
                        int digits = 0;
                        while (current_state_counter_temp > 0) {
                            digits++;
                            current_state_counter_temp /= 10;
                        }
                        int* tmp = (int*) malloc(sizeof(int) * digits); // check if this works later when you get to value
                        arr_size++;
                        arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                        if (digits == 2) {
                            arr_size += 1;
                            arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                        } 
                        if (digits == 3) {
                            arr_size += 2;
                            arr = (unsigned char*) realloc(arr, sizeof(unsigned char) * arr_size);
                        }
                        int index_temp = 0;
                        while (current_state_counter > 0) {
                            int digit = current_state_counter % 10;
                            tmp[index_temp] = digit;
                            index_temp++;
                            current_state_counter /= 10;
                        }
                        arr[arr_index] = digits +'0';
                        arr_index ++;
                        for (int l = 0; l < digits; l ++) {
                            arr[arr_index] = tmp[digits - l - 1] + '0';
                            arr_index ++;
                        }
                        free(tmp);
                        current_state_counter = 0;
                        innner_state_times ++;
                        if (innner_state_times == 2) {
                            state = 0; //0
                            inner_state = 0;
                            innner_state_times = 0;
                            in_method = False;
                        } else {
                            inner_state = 0; //0
                        }
                    }
                }
                else if (inner_state == 2) { // reg
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                    if (count == 2) {
                        count = -1;
                        arr[arr_index] = current_state_counter + '0';
                        arr_index ++;
                        current_state_counter = 0;
                        innner_state_times ++;

                        if (innner_state_times == 2) {
                            state = 0; //0
                            inner_state = 0;
                            innner_state_times = 0;
                            in_method = False;

                        } else {
                            inner_state = 0; //0
                        }
                    }
                }
                else if (inner_state == 3) { // reading 5 bits for STACK ref
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                    if (count == 4) {
                        count = -1;
                        if (current_state_counter < 26) {
                            arr[arr_index] = current_state_counter + 65;
                        } else if (current_state_counter >= 26) {
                            arr[arr_index] = current_state_counter - 26 + 97;
                        }
                        // for (int m = 0; m < 32; m++) {
                        //     printf("%c\n", symbol[m]);
                        //     if (symbol[m] == current_state_counter) {
                        //         if (m < 26) {
                        //             arr[arr_index] = m + 65;
                        //         } else if (m >= 26) {
                        //             arr[arr_index] = m - 26 + 97;
                        //         }
                        //     }
                        // }
                        arr_index ++;
                        current_state_counter = 0;
                        innner_state_times ++;
                        if (innner_state_times == 2) {
                            state = 0; //0
                            inner_state = 0;
                            innner_state_times = 0;
                            in_method = False;
                        } else {
                            inner_state = 0; //0
                        }
                    }
                }

                else if (inner_state == 4) { // 5 bit for pointer
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                    if (count == 4) {
                        count = -1;
                        if (current_state_counter < 26) {
                            arr[arr_index] = current_state_counter + 65;
                        } else if (current_state_counter >= 26) {
                            arr[arr_index] = current_state_counter - 26 + 97;
                        }
                        // for (int m = 0; m < 32; m++) {
                        //     if (symbol[m] == current_state_counter) {
                        //         if (m < 26) {
                        //             arr[arr_index] = m + 65;
                        //         } else if (m >= 26) {
                        //             arr[arr_index] = m - 26 + 97;
                        //         }
                        //     }
                        // }
                        arr_index ++;
                        current_state_counter = 0;
                        innner_state_times ++;
                        if (innner_state_times == 2) {
                            state = 0; //0
                            inner_state = 0;
                            innner_state_times = 0;
                            in_method = False;
                        } else {
                            inner_state = 0; //0
                        }
                    }
                }
            
            }

            else if (state == 7) {
                if (inner_state == 0) {
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}

                    if (count == 1) {
                        if (current_state_counter == 1) { // reg address
                            arr[arr_index] = 'x';
                            copy_char(2, &inner_state, &count, &arr_index, &current_state_counter, &innner_state_times);
                        }
                    }
                }
                else if (inner_state == 2) { // reg
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                    if (count == 2) {
                        count = -1;
                        arr[arr_index] = current_state_counter + '0';
                        arr_index ++;
                        current_state_counter = 0;
                        innner_state_times ++;

                        if (innner_state_times == 2) {
                            state = 0; //0
                            inner_state = 0;
                            innner_state_times = 0;
                            in_method = False;

                        } else {
                            inner_state = 0; //0
                        }
                    }
                }
            }

            else if (state == 8) {
                if (inner_state == 0) {
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}

                    if (count == 1) {
                        if (current_state_counter == 1) { // reg address
                            arr[arr_index] = 'x';
                            copy_char(2, &inner_state, &count, &arr_index, &current_state_counter, &innner_state_times);
                        }
                    }
                }
                else if (inner_state == 2) { // reg
                    if (IS_BIT_SET(buffer[size - i - 1], j)) {current_state_counter += power(2, count);}
                    if (count == 2) {
                        count = -1;
                        arr[arr_index] = current_state_counter + '0';
                        arr_index ++;
                        current_state_counter = 0;
                        innner_state_times ++;

                        if (innner_state_times == 2) {
                            state = 0; //0
                            inner_state = 0;
                            innner_state_times = 0;
                            in_method = False;

                        } else {
                            inner_state = 0; //0
                        }
                    }
                }
            }




            count ++;
            total_count ++;
        }
    }

    unsigned char* final_arr = (unsigned char*) malloc(sizeof(int) * arr_size);
    int final_arr_index = 0;

    //struct operand** instructions = (struct operand**) malloc(sizeof(struct operand))

    for(int i = 0; i < arr_size; i ++) {
        //printf("%c", arr[i]);
        printf("%c", arr[arr_size - i - 1]);
    }

    printf("\n");
    //int function_index = -1;
    int symbol[32];
    for (int u = 0; u < 32; u ++) {
        symbol[u] = 33;
    }
    int symbol_index = 0;
    for(int i = 0; i < arr_size; i ++) {
        //printf("%c", arr[i]);
        //printf("%c", arr[arr_size - i - 1]);

        if (arr[arr_size - i - 1] == '|') {
            final_arr[final_arr_index] = '|';
            i++;
            final_arr_index++;
            final_arr[final_arr_index] = arr[arr_size - i - 1];
            final_arr_index++;
            symbol_index = 0;
            for (int u = 0; u < 32; u ++) {
                symbol[u] = 33;
            }
            //function_index++;
        } 
        else if (arr[arr_size - i - 1] == 'm') {
            final_arr[final_arr_index] = 'm';
            final_arr_index++;
            int tmp = i;
            
            for (int j = 0; j < 4; j ++) {
                tmp --;
                if (arr[arr_size - tmp - 1] == 'v') {
                    final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                    final_arr_index++;
                    tmp--;
                    int digits = arr[arr_size - tmp - 1] - '0';
                    final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                    final_arr_index++;
                    
                    for (int h = 0; h < digits; h ++) {
                        tmp --;
                        final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                        final_arr_index++;
                    }
                    j++;
                } 
                else if (arr[arr_size - tmp - 1] == 's') {
                    final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                    final_arr_index++;
                    tmp--;
                    int checkSymbolExist = 0;

                    //if (arr[arr_size - tmp - 1] - 65 < 26) {
                    for (int m = 0; m < 32; m ++) {
                        if (symbol[m] == arr[arr_size - tmp - 1]) {
                            if (m < 26) {
                                final_arr[final_arr_index] = m + 65;
                                checkSymbolExist = 1;
                            }
                            else {
                                final_arr[final_arr_index] = m -26 + 97;
                                checkSymbolExist = 1;
                            }
                        }
                    }

                    if (!checkSymbolExist) {
                        symbol[symbol_index] = arr[arr_size - tmp - 1];
                        if (symbol_index < 26) {
                            final_arr[final_arr_index] = symbol_index + 65;
                        } else if (symbol_index >= 26) {
                            final_arr[final_arr_index] = symbol_index - 26 + 97;
                        }
                        symbol_index++;
                    }
                    final_arr_index++;
                    j++;
                }
                else if (arr[arr_size - tmp - 1] == 'p') {
                    final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                    final_arr_index++;
                    tmp--;
                    for (int m = 0; m < 32; m ++) {
                        if (symbol[m] == arr[arr_size - tmp - 1]) {
                            if (m < 26) {
                                final_arr[final_arr_index] = m + 65;
                            }
                            else {
                                final_arr[final_arr_index] = m -26 + 97;
                            }
                        }
                    }
                    final_arr_index++;
                    j++;
                }
                
                else {
                    final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                    final_arr_index++; 
                }
            }
        }
        else if (arr[arr_size - i - 1] == 'c') {
            final_arr[final_arr_index] = 'c';
            final_arr_index++;
            int tmp = i;

            for (int j = 0; j < 2; j ++) {
                tmp --;
                if (arr[arr_size - tmp - 1] == 'v') {
                    final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                    final_arr_index++;
                    tmp--;
                    int digits = arr[arr_size - tmp - 1] - '0';
                    final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                    final_arr_index++;
                    
                    for (int h = 0; h < digits; h ++) {
                        tmp --;
                        final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                        final_arr_index++;
                    }
                    j++;
                } else {
                    final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                    final_arr_index++; 
                }
            }
        }
        else if (arr[arr_size - i - 1] == 'r') {
            final_arr[final_arr_index] = 'r';
            final_arr_index++;
            int tmp = i;

            for (int j = 0; j < 4; j ++) {
                tmp --;
                if (arr[arr_size - tmp - 1] == 's') {
                    final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                    final_arr_index++;
                    tmp--;
                    int checkSymbolExist = 0;

                    //if (arr[arr_size - tmp - 1] - 65 < 26) {
                    for (int m = 0; m < 32; m ++) {
                        if (symbol[m] == arr[arr_size - tmp - 1]) {
                            if (m < 26) {
                                final_arr[final_arr_index] = m + 65;
                                checkSymbolExist = 1;
                            }
                            else {
                                final_arr[final_arr_index] = m -26 + 97;
                                checkSymbolExist = 1;
                            }
                        }
                    }

                    if (!checkSymbolExist) {
                        symbol[symbol_index] = arr[arr_size - tmp - 1];
                        if (symbol_index < 26) {
                            final_arr[final_arr_index] = symbol_index + 65;
                        } else if (symbol_index >= 26) {
                            final_arr[final_arr_index] = symbol_index - 26 + 97;
                        }
                        symbol_index++;
                    }
                    final_arr_index++;
                    j++;
                }

                else if (arr[arr_size - tmp - 1] == 'p') {
                    final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                    final_arr_index++;
                    tmp--;
                    for (int m = 0; m < 32; m ++) {
                        if (symbol[m] == arr[arr_size - tmp - 1]) {
                            if (m < 26) {
                                final_arr[final_arr_index] = m + 65;
                            }
                            else {
                                final_arr[final_arr_index] = m -26 + 97;
                            }
                        }
                    }
                    final_arr_index++;
                    j++;
                }
                else {
                    final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                    final_arr_index++; 
                }

                
            }
        }
        else if (arr[arr_size - i - 1] == 'a') {
            final_arr[final_arr_index] = 'a';
            final_arr_index++;
            int tmp = i;

            for (int j = 0; j < 4; j ++) {
                tmp--;
                final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                final_arr_index++;
            }
        }
        else if (arr[arr_size - i - 1] == 'o') {
            final_arr[final_arr_index] = 'o';
            final_arr_index++;
            int tmp = i;

            for (int j = 0; j < 2; j ++) {
                tmp --;
                if (arr[arr_size - tmp - 1] == 'v') {
                    final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                    final_arr_index++;
                    tmp--;
                    int digits = arr[arr_size - tmp - 1] - '0';
                    final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                    final_arr_index++;
                    
                    for (int h = 0; h < digits; h ++) {
                        tmp --;
                        final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                        final_arr_index++;
                    }
                    j++;
                } 
                else if (arr[arr_size - tmp - 1] == 's') {
                    final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                    final_arr_index++;
                    tmp--;
                    int checkSymbolExist = 0;

                    //if (arr[arr_size - tmp - 1] - 65 < 26) {
                    for (int m = 0; m < 32; m ++) {
                        if (symbol[m] == arr[arr_size - tmp - 1]) {
                            if (m < 26) {
                                final_arr[final_arr_index] = m + 65;
                            }
                            else {
                                final_arr[final_arr_index] = m -26 + 97;
                            }
                        }
                    }

                    final_arr_index++;
                    j++;
                }
                else if (arr[arr_size - tmp - 1] == 'p') {
                    final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                    final_arr_index++;
                    tmp--;
                    for (int m = 0; m < 32; m ++) {
                        if (symbol[m] == arr[arr_size - tmp - 1]) {
                            if (m < 26) {
                                final_arr[final_arr_index] = m + 65;
                            }
                            else {
                                final_arr[final_arr_index] = m -26 + 97;
                            }
                        }
                    }
                    final_arr_index++;
                    j++;
                }
                else {
                    final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                    final_arr_index++; 
                }
                
            }
        }
        else if (arr[arr_size - i - 1] == 'n') {
            final_arr[final_arr_index] = 'n';
            final_arr_index++;
            int tmp = i;

            for (int j = 0; j < 2; j ++) {
                tmp--;
                final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                final_arr_index++;
            }
        }
        else if (arr[arr_size - i - 1] == 'e') {
            final_arr[final_arr_index] = 'e';
            final_arr_index++;
            int tmp = i;

            for (int j = 0; j < 2; j ++) {
                tmp--;
                final_arr[final_arr_index] = arr[arr_size - tmp - 1];
                final_arr_index++;
            }
        }
        else if (arr[arr_size - i - 1] == 'R') {
            final_arr[final_arr_index] = arr[arr_size - i - 1];
            final_arr_index++;
        }
    }

    for (int j = 0; j < arr_size; j ++) {
        printf("%c", final_arr[j]);
    }
    printf("\n");


    for (int i = 0; i < arr_size; i ++) {
        if (final_arr[i] == '|') {
            i ++;
            printf("FUNC LABEL %d\n", final_arr[i] - '0');
        } 
        else if (final_arr[i] == 'm') {   // mising pointer print
            printf("    MOV");
            i++;
            int u = i + 4;
            int j = i;
            for (j = i; j < u; j ++) {
                if (final_arr[j] == 'x') {
                    j ++;
                    printf(" REG %d", final_arr[j] - '0');
                }
                else if (final_arr[j] == 'v') {
                    
                    j ++;
                    if (final_arr[j] == '1') {
                        j ++;
                        printf(" VAL %d", final_arr[j] - '0');
                    } 
                    else if (final_arr[j] == '2') {
                        int val_total = 0;
                        j ++;
                        val_total += ((final_arr[j] - '0') * 10);
                        j ++;
                        val_total += final_arr[j] - '0';
                        printf(" VAL %d", val_total);
                    }
                    else if (final_arr[j] == '3') {
                        int val_total = 0;
                        j ++;
                        val_total += ((final_arr[j] - '0') * 100);
                        j ++;
                        val_total += ((final_arr[j] - '0') * 10);
                        j ++;
                        val_total += final_arr[j] - '0';
                        printf(" VAL %d", val_total);
                    }
                }
                else if (final_arr[j] == 's') {
                    j++;
                    printf(" STK %c", final_arr[j]);
                }
                else if (final_arr[j] == 'p') {
                    j++;
                    printf(" PTR %c", final_arr[j]);
                }
            }
            i = j;
            i --;

            printf("\n");
        }
        else if (final_arr[i] == 'c') {
            printf("    CAL");
            i ++;
            int u = i + 2;
            int j = i;
            for (j = i; j < u; j ++) {
                if (final_arr[j] == 'x') {
                    j ++;
                    printf(" REG %d", final_arr[j] - '0');
                }
                else if (final_arr[j] == 'v') {
                    
                    j ++;
                    if (final_arr[j] == '1') {
                        j ++;
                        printf("Hey\n");
                        printf(" VAL %d", final_arr[j] - '0');
                    } 
                    else if (final_arr[j] == '2') {
                        int val_total = 0;
                        j ++;
                        val_total += ((final_arr[j] - '0') * 10);
                        j ++;
                        val_total += final_arr[j] - '0';
                        printf(" VAL %d", val_total);
                    }
                    else if (final_arr[j] == '3') {
                        int val_total = 0;
                        j ++;
                        val_total += ((final_arr[j] - '0') * 100);
                        j ++;
                        val_total += ((final_arr[j] - '0') * 10);
                        j ++;
                        val_total += final_arr[j] - '0';
                        printf(" VAL %d", val_total);
                    }
                }
            }
            i = j;
            i --;

            printf("\n");
        }
        else if (final_arr[i] == 'r') {
            printf("    REF");
            i ++;
            int u = i + 4;
            int j = i;
            for (j = i; j < u; j ++) {
                if (final_arr[j] == 'x') {
                    j ++;
                    printf(" REG %d", final_arr[j] - '0');
                    //printf("%c %c", final_arr[j+1], final_arr[j+2]);
                }
                else if (final_arr[j] == 's') {
                    j++;
                    printf(" STK %c", final_arr[j]);
                    //printf("HEY");
                }
                else if (final_arr[j] == 'p') {
                    j++;
                    printf(" PTR %c", final_arr[j]);
                }
            }
            i = j;
            i --;
            
            printf("\n");
        }
        else if (final_arr[i] == 'a') {
            printf("    ADD REG %d REG %d\n", final_arr[i+2] - '0', final_arr[i+4] - '0');
            i += 4;
        }
        else if (final_arr[i] == 'o'){
            printf("    PRINT");
            i ++;
            int u = i + 2;
            int j = i;
            for (j = i; j < u; j ++) {
                if (final_arr[j] == 'x') {
                    j ++;
                    printf(" REG %d", final_arr[j] - '0');
                }
                else if (final_arr[j] == 'v') {
                    
                    j ++;
                    if (final_arr[j] == '1') {
                        j ++;
                        printf(" VAL %d", final_arr[j] - '0');
                    } 
                    else if (final_arr[j] == '2') {
                        int val_total = 0;
                        j ++;
                        val_total += ((final_arr[j] - '0') * 10);
                        j ++;
                        val_total += final_arr[j] - '0';
                        printf(" VAL %d", val_total);
                    }
                    else if (final_arr[j] == '3') {
                        int val_total = 0;
                        j ++;
                        val_total += ((final_arr[j] - '0') * 100);
                        j ++;
                        val_total += ((final_arr[j] - '0') * 10);
                        j ++;
                        val_total += final_arr[j] - '0';
                        printf(" VAL %d", val_total);
                    }
                }
                else if (final_arr[j] == 's') {
                    j++;
                    printf(" STK %c", final_arr[j]);
                }
                else if (final_arr[j] == 'p') {
                    j++;
                    printf(" PTR %c", final_arr[j]);
                }
            }
            i = j;
            i --;

            printf("\n");
        }
        else if (final_arr[i] == 'n') {
            printf("    NOT REG %d\n", final_arr[i+2] - '0');
            i += 2;
        }
        else if (final_arr[i] == 'e') {
            printf("    EQU REG %d\n", final_arr[i+2] - '0');
            i += 2;
        }
        else if (final_arr[i] == 'R') {
            printf("    RET\n");
        }
    }

    //printf("\n");

    free (buffer);
    free (arr);
    free (final_arr);
}