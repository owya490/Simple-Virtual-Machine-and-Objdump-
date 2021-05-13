#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum { // v = value; x = register; s = Stack, p = pointer
    MOV = 'm', 
    CAL = '?', //?
    RET = '!', //!
    REF = 'r', 
    ADD = '>', //>
    PRINT = 'o', // can clash with Stack names A- Z
    NOT = 'n',
    EQU = '<' //<
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

vm_cpu* new_cpu(unsigned char* memory, int instr_size);

void assign_type1(vm_cpu* c);

void assign_type2(vm_cpu* c);

void fetch(vm_cpu* c);

void find_function(vm_cpu* c, int f);

void execute(vm_cpu* c);

void run_cpu(vm_cpu* c);