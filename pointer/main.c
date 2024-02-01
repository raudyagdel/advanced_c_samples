//
// Created by raudy on 1/31/2024.
//

#include <stdlib.h>
#include <stdio.h>
#include "func.h"

int main() {
    int number = 1;
    const int const_number = 10;
    const int * pointer_to_const_number = &const_number;
    int * const const_pointer_to_number = &number;
    const int * const const_pointer_to_const_number = &const_number;
    int* pointer = (int *) *(int *) (&const_number);

    printf("number                          %d\n", number);
    printf("const_number                    %d\n", const_number);
    printf("pointer_to_const_number         %d\n", *pointer_to_const_number);
    printf("const_pointer_to_number         %d\n", *const_pointer_to_number);
    printf("const_pointer_to_const_number   %d\n", *const_pointer_to_const_number);
    printf("pointer                         %d\n", *&pointer);


    int n = 5;
    int (*INC_FUNC)(int*) = increment;
    int (*arrayFunc[])(int*) = {increment, decrement, INC_FUNC, equal};
    size_t func_len = sizeof(arrayFunc) / sizeof(arrayFunc[0]);

    for (int i = 0; i <func_len; i++) {
        printf("%d\n", arrayFunc[i](&n));
    }

        
    return EXIT_SUCCESS;
}