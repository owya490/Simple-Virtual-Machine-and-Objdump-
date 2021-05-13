#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "objdump.c"
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

// enum bool {
//     True = 1, 
//     False = 0
// };
typedef struct {
    unsigned char* mem;
    unsigned char ram[256];
    int max_ram;
    unsigned char pc; // reg0x07
    char sp; // reg0x06
    unsigned char ip; // reg0x05
    int instr_size;

    unsigned char reg[4]; //7 reg as last 0x07 is pc;
    int reg_val_or_address[4];
    instr inst;
    vtype vtype1;
    int val1;
    vtype vtype2;
    int val2;

    int error = 0;
} vm_cpu;

vm_cpu* new_cpu(unsigned char* memory, int instr_size) {
    vm_cpu* c = malloc(sizeof(vm_cpu));
    c->mem = memory;
    // need to store on ram
    //c->ram[256] = {'\0'};
    c->max_ram = 256;
    c->reg_val_or_address[0] = 0;
    c->reg_val_or_address[1] = 0;
    c->reg_val_or_address[2] = 0;
    c->reg_val_or_address[3] = 0;

    c->pc = -1; 
    c->sp = -1;
    c->ip = -1;
    c->instr_size = instr_size;
    // c->inst;
    // c->vtype1;
    // c->val1;
    // c->vtype2;
    // c->val2;

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
    //c->pc++;
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
    //c->pc++;
}
//[mx0v13] {'m', 'v', '3', '2', '5', '6', 'x', '0'};

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
        // use RET in run_cpu function while c->inst != RET
        case 'r' :
            c->inst = REF;
            c->pc++;
            assign_type1(c);
            c->pc++;
            assign_type2(c);
            //exit(0);
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
            //printf("i = %d\n", i);
            if (c->mem[i+1] - '0' == f) {
                //printf("i = %d\n", i);
                c->pc = i +1;
                //printf("cp : %d\n", c->pc);
                return;
            }
        }
    }
}

// if a NEW stack ref is created, just throw onto stack as letter given, and then if stack if refereced, scroll thorugh stack to find the same LETTER, 
// if SYMBOl scroll through from instruction pointer to stack pointer so within current stack frame, but if PTR can scroll through all other stack frames

//JKS STK and PTR are stored at the opposite end of RAM, and the function calls are stored on the front side - STK is first 32 of end ram char places, and PTR is next 32 so toatl is 64 end slots of ram dedicated for PTR AND STIK SYMBOSL

// need an stack structure to store the ip for the instruction array othe rthan stack
// if hit RET AND IP == 0, then program will terminate 
void execute(vm_cpu* c) {
    
    switch (c->inst) {
        case MOV:
            if (c->vtype1 == REG) {
                if (c->vtype2 == VAL) {
                    c->reg[c->val1] = c->val2;
                    //c->reg_val_or_address[c->val1] = 0;
                }
                else if (c->vtype2 == REG) {
                    // if (c->reg_val_or_address[c->val2] == 1) {
                    //     c->reg[c->val1] = c->ram[c->reg[c->val2]];
                    //     c->reg_val_or_address[c->val1] = 0;
                    // }
                    // else if (c->reg_val_or_address[c->val2] == 0) {
                    //     c->reg[c->val1] = c->reg[c->val2];
                    //     c->reg_val_or_address[c->val1] = 0;
                    // }
                    c->reg[c->val1] = c->reg[c->val2];

                } 
                else if (c->vtype2 == STK) {
                    c->reg[c->val1] = c->ram[c->ip + 1 + c->val2];
                    //c->reg_val_or_address[c->val1] = 0;
                    //c->reg[c->val1] = c->ram[c->max_ram - 1 - c->val2];
                }
                else if (c->vtype2 == PTR) {
                    c->reg[c->val1] = c->ram[c->ram[c->ip + 1 + c->val2]];
                    // c->reg[c->val1] = c->ram[c->ip + 1 + c->val2];
                    // c->reg_val_or_address[c->val1] = 0;
                }
            } 
            else if (c->vtype1 == STK) {
                if (c->ram[c->ip + 1 + c->val1] == '\0') {
                    c->sp++;
                    if (c->sp >= c->max_ram) {
                        fprintf(stderr, "Ram Buffer Overflow");
                        c->error = 1;
                        return;
                    }
                }
                if (c->vtype2 == VAL) {
                    c->ram[c->ip + 1 + c->val1] = c->val2;
                    //c->ram[c->max_ram -1 - c->val1] = c->val2;
                }
                else if (c->vtype2 == REG) {
                    // if (c->reg_val_or_address[c->val2] == 1) {
                    //     c->ram[c->ip +1 + c->val1] = c->ram[c->reg[c->val2]];
                    // }
                    // else if (c->reg_val_or_address[c->val2] == 0) {
                    //     c->ram[c->ip +1 + c->val1] = c->reg[c->val2];
                    //     //c->reg_val_or_address[c->val1] = 1;
                    // }
                    c->ram[c->ip +1 + c->val1] = c->reg[c->val2];
                    //c->ram[c->max_ram -1 - c->val1] = c->reg[c->val2];
                }
                else if (c->vtype2 == STK) {
                    //c->sp ++;
                    c->ram[c->ip + 1 + c->val1] = c->ram[c->ip + 1 + c->val2];
                    // c->ram[c->max_ram -1 - c->val1] = c->ram[c->max_ram -1 - c->val2];
                }
                else if (c->vtype2 == PTR){
                    c->ram[c->ip + 1 + c->val1] = c->ram[c->ram[c->ip + 1 + c->val2]];
                    //c->ram[c->ip + 1 + c->val1] = c->ram[c->ip + 1 + c->val2];
                    //c->ram[c->ip + 1 + c->val1] = 
                    //c->ram[c->max_ram -1 - c->val1] = c->ram[c->max_ram - 1 - c->ram[c->max_ram - 1 - 32 - c->val2]]; //ram[max_ram - 1 - val stored in PTR]

                }
                //printf("sp: %d\n", c->sp);
                //exit(0);

            }
            else if (c->vtype1 == PTR) {
                if (c->vtype2 == VAL) {
                    c->ram[c->ram[c->ip + 1 + c->val1]] = c->val2;
                    //c->ram[c->ip + 1 + c->val1] = c->val2;
                }
                else if (c->vtype2 == REG) {
                    // if (c->reg_val_or_address[c->val2] == 1) {
                    //     c->ram[c->ip +1 + c->val1] = c->ram[c->reg[c->val2]];
                    // }
                    // else if (c->reg_val_or_address[c->val2] == 0) {
                    //     c->ram[c->ip +1 + c->val1] = c->reg[c->val2];
                    //     //c->reg_val_or_address[c->val1] = 1;
                    // }
                    c->ram[c->ram[c->ip + 1 + c->val1]] = c->reg[c->val2];
                    //c->ram[c->ip +1 + c->val1] = c->reg[c->val2];
                }
                else if (c->vtype2 == STK) {
                    c->ram[c->ram[c->ip + 1 + c->val1]] = c->ram[c->ip + 1 + c->val2];
                    //c->ram[c->ip + 1 + c->val1] = c->ram[c->ip + 1 + c->val2];
                    //c->ram[c->max_ram- -1 - 32 - c->val1] = c->val2;
                }
                else if (c->vtype2 == PTR){
                    c->ram[c->ram[c->ip + 1 + c->val1]] = c->ram[c->ram[c->ip + 1 + c->val2]];
                    //c->ram[c->ip + 1 + c->val1] = c->ram[c->ip + 1 + c->val2];
                }
            }
            break;
        
        case CAL:
        // will add the instruction pointer address to reg0x06 and then at the address on stack, store the previous instruction pointners address
            c->sp++;
            c->ram[c->sp] = c->ip;
            c->ip = c->sp;
            c->ram[c->max_ram-1] = c->pc;
            c->max_ram--;
            find_function(c, c->val1);
            //printf("sp: %d      ip: %d       pc: %d\n", c->sp, c->ip, c->pc);
            //exit(0);

            break;
        
        case REF:
            if (c->vtype1 == REG) {
                if (c->vtype2 == STK) {
                    c->reg[c->val1] = c->ip + 1 + c->val2;
                    //c->reg_val_or_address[c->val1] = 1;

                }
            }

        case ADD:
            if (c->vtype1 == REG) {
                if (c->vtype2 == REG) {
                    // if (c->reg_val_or_address[c->val1] == 1) {
                    //     c->reg[c->val1] = c->ram[c->reg[c->val1]] + c->reg[c->val2];
                    //     c->reg_val_or_address[c->val1] = 0;
                    // }
                    // else if (c->reg_val_or_address[c->val2] == 1) {
                    //     c->reg[c->val1] = c->ram[c->reg[c->val2]] + c->reg[c->val1];
                    //     c->reg_val_or_address[c->val1] = 0;
                    // }
                    // else if (c->reg_val_or_address[c->val1] == 1 && c->reg_val_or_address[c->val2] == 1) {
                    //     c->reg[c->val1] = c->ram[c->reg[c->val1]] + c->ram[c->reg[c->val2]];
                    //     c->reg_val_or_address[c->val1] = 0;
                    // }
                    // else if (c->reg_val_or_address[c->val1] == 0 && c->reg_val_or_address[c->val2] == 0) {
                    //     c->reg[c->val1] = c->reg[c->val1] + c->reg[c->val2];
                    //     c->reg_val_or_address[c->val1] = 0;
                    // }
                    c->reg[c->val1] = c->reg[c->val1] + c->reg[c->val2];
                }
            }
            break;
        
        case RET:
            //printf("sp: %d      ip: %d       pc: %d\n", c->sp, c->ip, c->pc);

            c->sp = c->ip - 1;
            c->ip = c->ram[c->ip];
            c->pc = c->ram[c->max_ram];
            c->max_ram ++;
            //printf("sp: %d      ip: %d       pc: %d\n", c->sp, c->ip, c->pc);
            break;

        case PRINT:
        if (c->vtype1 == REG) {
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
                    c->reg[c->val1] == 1; 
                } else {
                    c->reg[c->val1] == 0;
                }
            }
            break;
        
    }
}

void run_cpu(vm_cpu* c) {

    //while(c->inst != RET) { // while c->inst pointer == NULL or c->stack pointer == -1
    while(c->sp != -1) {
        fetch(c);
        switch(c->inst) {
            case MOV:
                printf("INST = %c    VTYPE1 = %c    VAL1 = %d    VTYPE2 = %c    VAL2 = %d\n", c->inst, c->vtype1, c->val1, c->vtype2, c->val2);
                break;
            case PRINT:
                printf("INST = %c    VTYPE1 = %c    VAL1 = %d\n", c->inst, c->vtype1, c->val1);
                break;
            case ADD:
                printf("INST = %c    VTYPE1 = %c    VAL1 = %d    VTYPE2 = %c    VAL2 = %d\n", c->inst, c->vtype1, c->val1, c->vtype2, c->val2);
                break;
            case RET:
                printf("INST = %c\n", c->inst);
                break;
            case REF:
                printf("INST = %c    VTYPE1 = %c    VAL1 = %d    VTYPE2 = %c    VAL2 = %d\n", c->inst, c->vtype1, c->val1, c->vtype2, c->val2);
                //printf("sp: %d      ip: %d       pc: %d\n", c->sp, c->ip, c->pc);
                //exit(0);
                break;
            case CAL:
                printf("INST = %c    VTYPE1 = %c    VAL1 = %d\n", c->inst, c->vtype1, c->val1);
                break;
        }
        // if (c->vtype1 == STK) {
        //     printf("LOl");
        // }
        execute(c);
        if (c->error) {
            return;
        }
    }
}



int main(int argc, char** argv) {
    struct ret r = objdump(argv[1]);
    printf("%s\n", r.arr_ret);
    printf("%d\n", r.arr_size);
    //unsigned char* arr_ret = objdump(argv[1]);

    //printf("%s\n", arr_ret);
    //free(arr_ret);
    //unsigned char arr[24] = {'|', '0', 'm', 's', 'A', 'v', '1', '2','r', 'x', '1', 's', 'A', 'm', 'x', '0', 'x', '1', 'a', 'x', '0', 'x', '1', 'R'};
    //unsigned char arr[24] = {'|', '0', 'm', 's', 'A', 'v', '1', '8', 'r', 'x', '0', 's', 'A', 'm', 's', 'B', 'x', '0', 'm', 'x', '1', 'p', 'B', 'R'};
    unsigned char arr[68] = {'|', '0', 'm', 's', 'A', 'v', '1', '8', 'm', 's', 'B', 'v', '1', '7', 'r', 'x', '0', 's', 'A', 'c', 'v', '1', '7', 'r', 'x', '0', 's', 'B', 'c', 'v', '1', '7', 'o', 's', 'A', 'o', 's', 'B', 'R', '|', '7', 'm', 's', 'A', 'x', '0', 'm', 'x', '0', 'p', 'A', 'm', 'x', '1', 'v', '1', '2', 'a', 'x', '0', 'x', '1', 'm', 'p', 'A', 'x', '0', 'R'};
    //unsigned char arr[16] = {'|', '0', 'm', 's', 'A', 'v', '1', '3', 'c', 'v', '1', '1', 'R', '|', '1', 'R'};
    vm_cpu* c = new_cpu(arr, 68);
    find_function(c, 0);
    //printf("FIRST PC: %d\n", c->pc);

    //printf("here: %d\n", c->pc);
    //fetch(c);
    //printf("INST = %c    VTYPE1 = %c    VAL1 = %d", c->inst, c->vtype1, c->val1);

    c->ip = 0;
    c->sp = 0;
    //printf("%d\n", c->ip);
    run_cpu(c);
    if (c->error) {
        return 1;
    }

    //c->ram[1] = 2;


    //printf("%d\n", c->ram[1]);
    //printf("%d\n", c->reg[1]);
    // if (c->ram[200] == '\0') {
    //     printf("LO::\n");
    // }
    // printf("%c\n", c->ram[200]);
    //printf("%c", RET);
    //find_function(c, 2);
    //printf("%d\n", c->pc);
    //fetch(c);
    //find_function(c, 0);
    //printf("%d", c->pc);
    //printf("%d\n", c->val1);
    //printf("%d\n", c->val2);
    //free (arr_ret);
    free(r.arr_ret);
    free(c);
}

// need to do a finder for specific func - e.g. func label 0 or wen call is used to find the other func