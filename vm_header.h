/*************************************************************************************
*    12 April 2021
*    COMP2017 Assignment 2 - VM
*    Owen Yang
*    500550430
*    DESCRIPTION: HEADER FILE FOR vm.c
*************************************************************************************/

#ifndef V
#define V

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*************************************************************************************
* ENUM DECLARATIONS.
*************************************************************************************/

typedef enum { 
    MOV = 'm', 
    CAL = '?', 
    RET = '!', 
    REF = 'r', 
    ADD = '>', 
    PRINT = 'o', 
    NOT = 'n',
    EQU = '<' 
}instr;

typedef enum {
    VAL = 'v', 
    REG = 'r', 
    STK = 's', 
    PTR = 'p'
}vtype;

/*************************************************************************************
* STRUCT DECLARATIONS.
*************************************************************************************/

typedef struct {
    /*************************************************************************************
     * STORES ALL INFORMATION OF VIRTUAL MACHINE.
     *      mem: the memory of instructions
     *      ram: the stack itself held within the machines ram
     *      max_ram: the maximum amount of ram for the state of the machine stored in register 4
     *      pc: program counter stored in register 7
     *      sp: stack pointer stored in register 6
     *      ip: instruction pointer stored in register 5
     *      instr_size: the size of instructions in memory
     * 
     *      reg[4]: the 4 accessible registers
     * 
     *      inst: instruction given
     *      vtype1: the type of command for command 1
     *      val1: value of command 1
     *      vtype2: the type of command for command 2
     *      val2: value of command 2
     *
     *************************************************************************************/
    unsigned char* mem;
    unsigned char ram[256];
    unsigned char max_ram; // reg0x04
    unsigned char pc; // reg0x07
    unsigned char sp; // reg0x06
    unsigned char ip; // reg0x05
    int instr_size;

    unsigned char reg[4]; //7 reg as last 0x07 is pc;
    instr inst;
    vtype vtype1;
    int val1;
    vtype vtype2;
    int val2;


} vm_cpu;

/*************************************************************************************
* FUNCTION DECLARATIONS.
*************************************************************************************/

/*
* Constructor for VM
*/
vm_cpu* new_cpu(unsigned char* memory, int instr_size);

/*
* Assigning Command1 type and value
*/
void assign_type1(vm_cpu* c);

/*
* Assigning Command2 type and value
*/
void assign_type2(vm_cpu* c);

/*
* Fetching the next operand command
*/
void fetch(vm_cpu* c);

/*
* Finds the function requested in the instruction array
*/
void find_function(vm_cpu* c, int f);

/*
* Executes fetched Operand command
*/
void execute(vm_cpu* c);

/*
* Runs the VM CPU
*/
void run_cpu(vm_cpu* c);

#endif