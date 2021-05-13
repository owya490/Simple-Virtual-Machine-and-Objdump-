#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>

#define IS_BIT_SET(BF, N) ((BF >> N) & 0x1)

#define i64 long long

enum bool {
    True = 1, 
    False = 0
};
struct rec
{
    int x,y,z;
};

int binToDec(char bin[8]) {

}

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

typedef enum { // V = value; X = register; S = Stack, P = pointer
    MOV = 'M', 
    CAL = 'C',
    RET = 'r',
    REF = 'R', 
    ADD = 'A',
    PRINT = 'P',
    NOT = 'N',
    EQU = 'E'
}instr;

struct operand {
    instr instruct;
    int val1;
    int val2;
};


int main() {

    FILE * f = fopen("tests/sample_program.x2017", "rb");

    //unsigned char buffer[10];

    //uint8_t buffer[10];

    int final[80];

    struct stat stats;
    //printf("%ld\n", stats.st_size);



    //char x[20];

    //fgets(a, 1, f);

    stat("tests/sample_program.x2017", &stats);
    //printf("%ld\n", stats.st_size);
    int size = stats.st_size;

    uint8_t* buffer = (uint8_t *) malloc(sizeof(uint8_t) * size);


    int start = True;
    int count = 0;
    int method_count = 0;
    int instructions_index = 0;
    //void* instructions;

    fread(buffer, 1, size, f);

    struct operand** instructions;

    unsigned char* arr;
    int arr_size;
    int arr_index = 0;

    printf("size of %zu\n", sizeof(instr));

    for (int i = 0; i < size; i ++) {
        for (int j = 0; j < 8; j ++) {
            //(IS_BIT_SET(buffer[size - i - 1], j)) ? printf("+") : printf("-");

            if (start && count < 5) {
                if (IS_BIT_SET(buffer[size - i - 1], j)) {method_count += power(2, j);}
                if (count == 4) {
                    printf("%d", method_count);
                    start = False;
                    //instr* functions = (instr*) malloc(sizeof())
                    //struct RET *tmp;

                    arr = (unsigned char*) malloc(sizeof(unsigned char));
                    arr_size = 1;
                    arr[arr_index] = 'R';
                    //printf("\n%c\n", arr[0]);




















                    instructions = malloc(sizeof(struct operand));

                    struct operand* tmp = (struct operand*) malloc(sizeof(struct operand) * 10);
                    tmp->instruct= RET;
                    
                    //tmp->instruct = RET;
                    //struct operand tmp = {.instruct = RET};
                    // tmp->instruct = RET;
                    // tmp->val1 = 1;
                    if (tmp->instruct == RET) {
                        printf("hey");
                    }
                    instructions[0] = tmp;
                    // int try =1;
                    // arr[0] = try;
                    //instructions[instructions_index] = (struct RET*) &tmp;
                    //printf("%d \n", j);
                }

                //printf("%d", arr[0]);
            }

            if (instructions[0]->instruct == RET) {
                printf("loL");
            }

            //if ()






            count ++;
        }
    }

}



// struct MOV {
//     int reg;
//     int val;
// };

// struct CAL {

// };

// struct RET {
//     int val;
// };

// struct REF {
//     int stk_a;
//     int stk_b;
// };

// struct ADD {
//     int reg_a;
//     int reg_b;
// };

// struct PRINT {
//     int address;
// };

// struct NOT {
//     int reg;
// };

// struct EQU {
//     int reg;
// };









    //char elem[8];
    //char elem_final[8];

    // fread(buffer, 1, size, f);
    // for (int j = 0; j < size; j ++) {
    //     for (int i = 0 ; i < 8; i ++) {
    //         //printf("%d", buffer[8 - j -1]>>i&1);
    //         //printf("%d", buffer[size - j -1]>>i&1);
    //             // if (IS_BIT_SET(buffer[size - j - 1], i)) {
    //             //     printf("+");
    //             // } else {
    //             //     printf("-");
    //             // }
            
    //         if (start && count < 5) {
    //             //elem[count] =  (buffer[size - j -1]>>i&1) + '0';

    //             if (IS_BIT_SET(buffer[size - j - 1], i)) {
    //                 method_count += power(2, i);
    //             }
    //             //printf("%c", elem[count]);
    //             count ++;
    //             if (count == 5) {
    //                 // for (int x = 0; x < count; x++) {
    //                 //     elem_final[x] = elem[5-x-1];
    //                 // }
    //                 // elem_final[count] = '\0';
    //                 printf("%d", method_count);
    //                 start = False;
    //                 //char *ptr = '\0';
    //                 //method_count = strtol(elem_final, &ptr, 2);
    //                 //printf("%d\n", method_count);
    //             }
                
    //         }
    //         //final[(8*(j+1)) - i -1] = buffer[j]>>i&1;
    //     }
    // }
    
    //elem[count] = '\0';

    // while(fread(buffer, 1, size, f)!= 0) {
    //     for (int j = 0; j < size; j ++) {
    //         for (int i = 0 ; i < 8; i ++) {
    //             //printf("%d", buffer[8 - j -1]>>i&1);
    //             //printf("%d", buffer[size - j -1]>>i&1);

                
    //             if (start && count < 5) {
    //                 elem[count] =  (buffer[size - j -1]>>i&1) + '0';
    //                 printf("%c", elem[count]);
    //                 count ++;
    //                 if (count == 5) {
    //                     for (int x = 0; x < count; x++) {
    //                         elem_final[x] = elem[5-x-1];
    //                     }
    //                     elem_final[count] = '\0';

    //                     char *ptr = '\0';
    //                     method_count = strtol(elem_final, &ptr, 2);
    //                     printf("%d\n", method_count);
    //                 }
                    
    //             }
    //             //final[(8*(j+1)) - i -1] = buffer[j]>>i&1;
    //         }
    //     }
    // }
    // elem[count] = '\0';
    //printf("%s", elem_final);

    // for (int i = 0; i < 80; i ++) {
    //     printf("%d", final[i]);
    // }




    // for (int j = 0; j < 256; j ++) {
    //     for (int i = 0 ; i < 8; i ++) {
    //         printf("%d", buffer[j]>>i&1);

    //     }
    // }


    //uint8_t b;

    //b = a&0x1;

    //printf("%d", b);

    //printf("%d", a);










//     int counter;
//     FILE *ptr_myfile;
//     struct rec my_record;

//     ptr_myfile=fopen("tests/sample_program.x2017","rb");
//     if (!ptr_myfile)
//     {
//         printf("Unable to open file!");
//         return 1;
//     }

//     fseek(ptr_myfile, sizeof(struct rec), SEEK_END);
//     rewind(ptr_myfile);

//     for ( counter=1; counter <= 10; counter++)
//     {
//         fread(&my_record,sizeof(struct rec),1,ptr_myfile);
//         printf("%d\n",my_record.x);
//     }
//     fclose(ptr_myfile);
//     return 0;
// }
























// #define SIZE 256

// void hexDump(size_t, void *, int);

// int main(int argc, char *argv[])
// {
//     if (argc != 2)
//     {
//         fprintf(stderr, "Usage: %s file\n", argv[0]);
//         exit(EXIT_FAILURE);
//     }
//     FILE *myfile = fopen(argv[1], "rb");
//     if (myfile == 0)
//     {
//         fprintf(stderr, "%s: failed to open file '%s' for reading\n", argv[0], argv[1]);
//         exit(EXIT_FAILURE);
//     }

//     unsigned char buffer[SIZE];
//     size_t n;
//     size_t offset = 0;
//     while ((n = fread(buffer, 1, SIZE, myfile)) > 0)
//     {
//         hexDump(offset, buffer, n);
//         if (n < SIZE)
//             break;
//         offset += n;
//     }

//     fclose(myfile);
//     return 0;
// }

// void hexDump(size_t offset, void *addr, int len)
// {
//     int i;
//     unsigned char bufferLine[17];
//     unsigned char *pc = (unsigned char *)addr;

//     for (i = 0; i < len; i++)
//     {
//         if ((i % 16) == 0)
//         {
//             if (i != 0)
//                 printf(" %s\n", bufferLine);
//             // Bogus test for zero bytes!
//             //if (pc[i] == 0x00)
//             //    exit(0);
//             printf("%08zx: ", offset);
//             offset += (i % 16 == 0) ? 16 : i % 16;
//         }

//         printf("%02x", pc[i]);
//         if ((i % 2) == 1)
//             printf(" ");

//         if ((pc[i] < 0x20) || (pc[i] > 0x7e))
//         {
//             bufferLine[i % 16] = '.';
//         }
//         else
//         {
//             bufferLine[i % 16] = pc[i];
//         }

//         bufferLine[(i % 16) + 1] = '\0';
//     }

//     while ((i % 16) != 0)
//     {
//         printf("  ");
//         if (i % 2 == 1)
//             putchar(' ');
//         i++;
//     }
//     printf(" %s\n", bufferLine);

// }