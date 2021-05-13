# Simple-Virtual-Machine-and-Objdump-
A Simple Virtual Machine and Objdump coded in C meant to emulate a CPU using register and a Stack Data Structure to perform operations.

• RAM - Contains 28 addresses of 1 byte each
• Register Bank - 8 registers of 1 byte each
• Program Code - Memory required to store the program to be executed.

**Arguments**
are expressed as one of four different types, while operation codes are selected from a table. The
arguments of each function precede the opcode and are expressed as follows:

  ([Second Value][Second Type])([First Value][First Type])[Operation Code]
  
A collection of these operations will form a function.
The type is a two bit field and specifies the type of the preceding value.

  00 - value: 1 byte long. The value in the preceding 8 bits should be interpreted as a single byte
  value.
  
  01 - register address: 3 bits long. This address refers to one of the eight fixed registers
  
  10 - stack symbol: 5 bits long. This refers to a particular symbol within the current stack frame.
  
  11 - pointer valued: 5 bits long. This treats the contents of the address referred to by a particular
  symbol within the current stack frame as a variable. Pointers may reference variables on
  different stack frames.
  
The second address and address type field is optional and will not be required for unary operations.
The address type specifies whether it is a stack address, a value, a register address or a pointer to
another stack address. Stack addresses are 7 bits long, register addresses are 3 bits long and values
are 8 bits long and pointer addresses are also 7 bits long.
A stack symbol is a value that is associated with an address on the stack; it is up to you to find
a cogent method of allocating stack space to symbols. Symbols only exist within the scope of the
current function; should two functions use the same symbol then they are not referencing the same
region of memory.
While the exact memory layout within the stack frame is open to interpretation, four registers are
reserved for special values.

  • 0x07 Stores the program counter.
  • 0x06 Will not be referenced by the program; this register exists for your personal use.
  • 0x05 Will not be referenced by the program; this register exists for your personal use.
  • 0x04 Will not be referenced by the program; this register exists for your personal use.
  
Registers in the range 0x00-0x03 are general purpose registers and may be explicitly referenced by a
program.
The opcodes associated with ‘x2017’ instructions are detailed below. You will need to read each of
the op-codes and implement the operation on the memory specified.

**Opcodes**:

  000 - [MOV A B] - Copies the value at some point B in memory to another point A in memory
  (register or stack). The destination may not be value typed.
  
  001 - [CAL A] - Calls another function the first argument is a single byte (using the VALUE type)
  containing the label of the calling function.
  
  010 - [RET] - Terminates the current function, this is guaranteed to always exist at the end of each
  function. There may be more than one RET in a function. If this function is the entry-point,
  then the program terminates.
  
  011 - [REF A B] - Takes a stack symbol B and stores its corresponding stack address in A.
  
  100 - [ADD A B] - Takes two register addresses and ADDs their values, storing the result in the first
  listed register.
  
  101 - [PRINT A] - Takes any address type and prints the contents to a new line of standard output
  as an unsigned integer.
  
  110 - [NOT A] - Takes a register address and performs a bitwise not operation on the value at that
  address. The result is stored in the same register
  
  111 - [EQU A] - Takes a register address and tests if it equals zero. The value in the register will be
  set to 1 if it is 0, or 0 if it is not. The result is stored in the same register.
  
The state of the registers is preserved between CAL and RET operations.
In the event that the execution of this program enters an undefined state; for example if the amount
of stack memory required to execute the program exceeds the RAM buffer, then you should print an
appropriate error to standard error and return 1 on exiting main.
The value of the program counter register should reference the current opcode. Your program should
increment the program counter before executing the associated instruction. You may wish to consider
what may happen when the program counter is modified during the execution of an instruction.

**Binary File Format**:
The first few bits of the file are padding bits to ensure that the total number of bits in the file accumulates
to a whole number of bytes. The number of padding bits will always be strictly less than one
byte.

The file is broken up into a number of functions. Each function is defined with a three bit header
dictating the ’label’ of the function and the number of arguments, and a five bit tail specifying the
number of instructions in the function. The function with the label 0 is the entry point and should be
executed first.

[Padding bits]
[function label (3 bits)]
  [OPCODE]
  [OPCODE]
  ...
  [RET]
  [Number of instructions (5 bits)]
[function label (3 bits)]
  [OPCODE]
  [OPCODE]
  ...
  [RET]
  [Number of instructions (5 bits)]
 
 
