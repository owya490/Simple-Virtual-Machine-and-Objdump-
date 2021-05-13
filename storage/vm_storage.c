#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include "objdump.c"
#include "objdump_header.h"
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
    VAL = 'v', 
    REG = 'r', 
    STK = 's', 
    PTR = 'p'
}vtype;


typedef struct {
    unsigned char* mem;
    unsigned char ram[256];
    unsigned char max_ram;
    unsigned char pc; // reg0x07
    unsigned char sp; // reg0x06
    unsigned char ip; // reg0x05
    int instr_size;

    unsigned char reg[4]; //7 reg as last 0x07 is pc;
    int reg_val_or_address[4];
    instr inst;
    vtype vtype1;
    int val1;
    vtype vtype2;
    int val2;


} vm_cpu;

vm_cpu* new_cpu(unsigned char* memory, int instr_size) {
    vm_cpu* c = malloc(sizeof(vm_cpu));
    c->mem = memory;
    for (int i = 0; i < 255; i ++) {
        c->ram[i] = '\0';
    }
    c->max_ram = 255;

    c->pc = -1; 
    c->sp = -1;
    c->ip = -1;
    c->instr_size = instr_size;

    return c;
}

void assign_type1(vm_cpu* c) {
    if (c->mem[c->pc] == 'v') {
        c->vtype1 = VAL;
        c->pc++;
        if (c->mem[c->pc] == '1') {
            c->pc++;
            c->val1 = c->mem[c->pc] - '0';
        } else if (c->mem[c->pc] == '2') {
            int val_total = 0;
            c->pc++;
            val_total += ((c->mem[c->pc] - '0') * 10);
            c->pc++;
            val_total += c->mem[c->pc] - '0';
            c->val1 = val_total;
        } else if (c->mem[c->pc] == '3') {
            int val_total = 0;
            c->pc++;
            val_total += ((c->mem[c->pc] - '0') * 100);
            c->pc++;
            val_total += ((c->mem[c->pc] - '0') * 10);
            c->pc++;
            val_total += c->mem[c->pc] - '0';
            c->val1 = val_total;
        }

    } else if (c->mem[c->pc] == 'x') {
        c->vtype1 = REG;
        c->pc++;
        c->val1 = c->mem[c->pc] - '0';
    } else if (c->mem[c->pc] == 's') {
        c->vtype1 = STK;
        c->pc++;
        if (c->mem[c->pc] - 'A' > 26) {
            c->val1 = (c->mem[c->pc] - 'a') + 26;
        } else {
            c->val1 = c->mem[c->pc] - 'A';
        }
    } else if (c->mem[c->pc] == 'p') {
        c->vtype1 = PTR;
        c->pc++;
        if (c->mem[c->pc] - 'A' > 26) {
            c->val1 = (c->mem[c->pc] - 'a') + 26;
        } else {
            c->val1 = c->mem[c->pc] - 'A';
        }
    }
}

void assign_type2(vm_cpu* c) {
    if (c->mem[c->pc] == 'v') {
        c->vtype2 = VAL;
        c->pc++;
        if (c->mem[c->pc] == '1') {
            c->pc++;
            c->val2 = c->mem[c->pc] - '0';
        } else if (c->mem[c->pc] == '2') {
            int val_total = 0;
            c->pc++;
            val_total += ((c->mem[c->pc] - '0') * 10);
            c->pc++;
            val_total += c->mem[c->pc] - '0';
            c->val2 = val_total;
        } else if (c->mem[c->pc] == '3') {
            int val_total = 0;
            c->pc++;
            val_total += ((c->mem[c->pc] - '0') * 100);
            c->pc++;
            val_total += ((c->mem[c->pc] - '0') * 10);
            c->pc++;
            val_total += c->mem[c->pc] - '0';
            c->val2 = val_total;
        }

    } else if (c->mem[c->pc] == 'x') {
        c->vtype2 = REG;
        c->pc++;
        c->val2 = c->mem[c->pc] - '0';
    } else if (c->mem[c->pc] == 's') {
        c->vtype2 = STK;
        c->pc++;
        if (c->mem[c->pc] - 'A' > 26) {
            c->val2 = (c->mem[c->pc] - 'a') + 26;
        } else {
            c->val2 = c->mem[c->pc] - 'A';
        }
    } else if (c->mem[c->pc] == 'p') {
        c->vtype2 = PTR;
        c->pc++;
        if (c->mem[c->pc] - 'A' > 26) {
            c->val2 = (c->mem[c->pc] - 'a') + 26;
        } else {
            c->val2 = c->mem[c->pc] - 'A';
        }
    }
}

void fetch(vm_cpu* c) {
    c->pc++;
    switch (c->mem[c->pc]) {
        case 'm' :
            c->inst = MOV;
            c->pc++;
            assign_type1(c);
            c->pc++;
            assign_type2(c);
            break;
        
        case 'c' :
            c->inst = CAL;
            c->pc++;
            assign_type1(c);
            break;
        case 'R' :
            c->inst = RET;
            break;
        case 'r' :
            c->inst = REF;
            c->pc++;
            assign_type1(c);
            c->pc++;
            assign_type2(c);
            break;
        case 'a' :
            c->inst = ADD;
            c->pc++;
            assign_type1(c);
            c->pc++;
            assign_type2(c);
            break;
        case 'o' :
            c->inst = PRINT;
            c->pc++;
            assign_type1(c);
            break;
        case 'n' :
            c->inst = NOT;
            c->pc++;
            assign_type1(c);
            break;
        case 'e' :
            c->inst = EQU;
            c->pc++;
            assign_type1(c);
            break;
    }
}

void find_function(vm_cpu* c, int f) {
    for (int i = 0; i < c->instr_size; i ++) {
        if (c->mem[i] == '|') {
            if (c->mem[i+1] - '0' == f) {
                c->pc = i +1;
                return;
            }
        }
    }
}

void execute(vm_cpu* c) {
    
    switch (c->inst) {
        case MOV:
            if (c->vtype1 == REG) {
                if (c->vtype2 == VAL) {
                    if (c->val1 == 7) {
                        c->pc = c->val2;
                    }
                    else {
                        c->reg[c->val1] = c->val2;
                    }
                }
                else if (c->vtype2 == REG) {
                    if (c->val1 == 7) {
                        c->pc = c->reg[c->val2];
                        //printf("%d\n", c->reg[c->val2]);
                        //exit(0);
                    }
                    else if (c->val2 == 7) {
                        c->reg[c->val1] = c->pc;
                    }
                    else {
                        c->reg[c->val1] = c->reg[c->val2];
                    }
                } 
                else if (c->vtype2 == STK) {
                    if (c->val1 == 7) {
                        c->pc = c->ram[c->ip + 1 + c->val2];
                    }
                    else {
                        c->reg[c->val1] = c->ram[c->ip + 1 + c->val2];
                    }
                    for (int b = 0; b < 9; b ++) {
                        //printf("RAM: %d\n", c->ram[b]);
                    }
                    //printf("REG 0 : %d\n", c->reg[0]);
                }
                else if (c->vtype2 == PTR) {
                    if (c->val1 == 7) {
                        c->pc = c->ram[c->ram[c->ip + 1 + c->val2]];
                    }
                    else {
                        c->reg[c->val1] = c->ram[c->ram[c->ip + 1 + c->val2]];
                        //printf("NEXT: %d\n", c->ram[c->ram[c->ip + 1 + c->val2]]);
                    }
                }
            } 
            else if (c->vtype1 == STK) {
                if (c->ram[c->ip + 1 + c->val1] == '\0') {
                    c->sp++;
                    if (c->sp >= c->max_ram) {
                        fprintf(stderr, "Buffer Overflow\n");
                        exit(1);
                    }
                }
                if (c->vtype2 == VAL) {
                    c->ram[c->ip + 1 + c->val1] = c->val2;
                }
                else if (c->vtype2 == REG) {
                    c->ram[c->ip +1 + c->val1] = c->reg[c->val2];
                    //printf("HEY%d\n", c->ip +1 + c->val1);
                    //printf("%d\n", c->ram[c->ip +1 + c->val1]);
                    for (int b = 0; b < 9; b ++) {
                        //printf("RAM: %d\n", c->ram[b]);
                    }
                }
                else if (c->vtype2 == STK) {
                    c->ram[c->ip + 1 + c->val1] = c->ram[c->ip + 1 + c->val2];
                }
                else if (c->vtype2 == PTR){
                    c->ram[c->ip + 1 + c->val1] = c->ram[c->ram[c->ip + 1 + c->val2]];
                    //printf("%d\n", c->ram[4]);
                    //printf("PTR MOVE: %d\n", c->ram[c->ip + 1 + c->val1]);
                    // for (int b = 0; b < 9; b ++) {
                    //     printf("RAM: %d\n", c->ram[b]);
                    // }
                }
            }
            else if (c->vtype1 == PTR) {
                if (c->vtype2 == VAL) {
                    c->ram[c->ram[c->ip + 1 + c->val1]] = c->val2;
                }
                else if (c->vtype2 == REG) {
                    c->ram[c->ram[c->ip + 1 + c->val1]] = c->reg[c->val2];
                    //printf("END: %d\n", c->ram[c->ram[c->ip + 1 + c->val1]]);
                }
                else if (c->vtype2 == STK) {
                    c->ram[c->ram[c->ip + 1 + c->val1]] = c->ram[c->ip + 1 + c->val2];
                }
                else if (c->vtype2 == PTR){
                    c->ram[c->ram[c->ip + 1 + c->val1]] = c->ram[c->ram[c->ip + 1 + c->val2]];
                }
            }
            break;
        
        case CAL:
            c->sp++;
            //printf("SP: %d\n", c->sp);
            c->ram[c->sp] = c->ip;
            c->ip = c->sp;
            c->ram[c->max_ram-1] = c->pc;
            c->max_ram--;
            find_function(c, c->val1);
            for (int b = 0; b < 9; b ++) {
                //printf("RAM: %d\n", c->ram[b]);
            }
            break;
        
        case REF:
            if (c->vtype1 == REG) {
                if (c->vtype2 == STK) {
                    c->reg[c->val1] = c->ip + 1 + c->val2;

                }
            }
            else if (c->vtype1 == STK) {
                if (c->ram[c->ip + 1 + c->val1] == '\0') {
                    c->sp++;
                    if (c->sp >= c->max_ram) {
                        fprintf(stderr, "Buffer Overflow\n");
                        exit(1);
                    }
                }
                c->ram[c->ip + 1 +c->val1] = c->ip + 1 + c->val2;
            }
            else if (c->vtype1 == PTR) {
                c->ram[c->ram[c->ip + 1 + c->val1]] = c->ip + 1 + c->val2;
            }

        case ADD:
            if (c->vtype1 == REG) {
                if (c->vtype2 == REG) {
                    if (c->val1 == 7) {
                        int count = 0;
                        int skipppingDone = 0;
                        for (int i = c->pc; i < c->instr_size; i ++) {
                            if (count == c->reg[c->val2]) {
                                skipppingDone = 1;

                                count ++;
                            }
                            if (c->mem[i] == 'm' || c->mem[i] == 'c' || c->mem[i] == 'R' || c->mem[i] == 'r' || c->mem[i] == 'a' || c->mem[i] == 'o' || c->mem[i] == 'n' || c->mem[i] == 'e') {
        
                                if (skipppingDone) {
                                    c->pc = i -1;
                                    break;
                                } 
                                else {
                                    count ++;
                                }
                            }
                        }
                    }
                    else {
                        c->reg[c->val1] = c->reg[c->val1] + c->reg[c->val2];
                        //printf("Reg 0: %d\n", c->reg[0]);
                    }
                }
            }
            break;
        
        case RET:
            c->sp = c->ip - 1;
            c->ip = c->ram[c->ip];
            c->pc = c->ram[c->max_ram];
            c->max_ram ++;
            for (int j = c->sp + 1; j < c->max_ram; j ++) {
                c->ram[j]= '\0';
            }

            break;

        case PRINT:
        if (c->vtype1 == VAL) {
            printf("%d\n", c->val1);
        }
        else if (c->vtype1 == REG) {
            printf("%d\n", c->reg[c->val1]);
        }
        else if (c->vtype1 == STK) {
            printf("%d\n", c->ram[c->ip + 1 + c->val1]);
        }
        else if (c->vtype1 == PTR) {
            printf("%d\n", c->ram[c->ram[c->ip + 1 + c->val1]]);
        }
            break;
        
        case NOT:
            if (c->vtype1 == REG) {
                c->reg[c->val1] = ~c->reg[c->val1];
            }
            break;
        
        case EQU:
            if (c->vtype1 == REG) {
                if (c->reg[c->val1] == 0) {
                    c->reg[c->val1] = 1; 
                } else {
                    c->reg[c->val1] = 0;
                }
            }
            break;
        
    }
}

void run_cpu(vm_cpu* c) {

    while(c->sp != 255) {
        fetch(c);
        // switch(c->inst) {
        //     case MOV:
        //         printf("INST = %c    VTYPE1 = %c    VAL1 = %d    VTYPE2 = %c    VAL2 = %d\n", c->inst, c->vtype1, c->val1, c->vtype2, c->val2);
        //         break;
        //     case PRINT:
        //         printf("INST = %c    VTYPE1 = %c    VAL1 = %d\n", c->inst, c->vtype1, c->val1);
        //         break;
        //     case ADD:
        //         printf("INST = %c    VTYPE1 = %c    VAL1 = %d    VTYPE2 = %c    VAL2 = %d\n", c->inst, c->vtype1, c->val1, c->vtype2, c->val2);
        //         printf("REG 1: %d     REG 2: %d\n", c->reg[c->val1], c->reg[c->val2]);
        //         break;
        //     case RET:
        //         printf("INST = %c\n", c->inst);
        //         break;
        //     case REF:
        //         printf("INST = %c    VTYPE1 = %c    VAL1 = %d    VTYPE2 = %c    VAL2 = %d\n", c->inst, c->vtype1, c->val1, c->vtype2, c->val2);
        //         //printf("sp: %d      ip: %d       pc: %d\n", c->sp, c->ip, c->pc);
        //         //exit(0);
        //         break;
        //     case CAL:
        //         printf("INST = %c    VTYPE1 = %c    VAL1 = %d\n", c->inst, c->vtype1, c->val1);
        //         break;
        //     case NOT:
        //         printf("INST = %c    VTYPE1 = %c    VAL1 = %d\n", c->inst, c->vtype1, c->val1);
        //         break;
        //     case EQU:
        //         printf("INST = %c    VTYPE1 = %c    VAL1 = %d\n", c->inst, c->vtype1, c->val1);
        //         break;
        // }
        execute(c);
    }
}



int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "No Arguments\n");
        exit(1);
    }

    // FILE *f = fopen(argv[1], "rb");

    // if (!f) {
    //     //fclose(f);
    //     fprintf(stderr, "No File\n");
    //     exit(1);
    // }

    // FILE* f;
    // if ((f = fopen(argv[1], "rb"))) {
    //     fclose(f);
    //     fprintf(stderr, "File doesnt exist\n");
    //     return 1;
    // }

    struct ret r = objdump(argv[1]);
    vm_cpu* c = new_cpu(r.arr_ret, r.arr_size);
    find_function(c, 0);

    c->ip = 0;
    c->ram[0] = 0;
    c->sp = 0;
    run_cpu(c);
    free(r.arr_ret);
    free(c);
}