#include <stdio.h>
#include <stdlib.h>

struct dori{
    char* name;
    int weight;
    char* colour;
};

int main() {
    struct dori british = {
        .name = "Dori",
        .weight = 3,
        .colour = "grey"
    };
    struct dori yellow = {.name = "yellow", .weight = 3.2, .colour = "orange"};
    printf("Hi! My name is %s, my weight is %d and I am %s\n", british.name, british.weight, british.colour);
    printf("%s, %d, %s\n", yellow.name, yellow.weight, yellow.colour);

    int a = 3;
    int * ptr = &a;
    printf("%p\n", ptr);

}