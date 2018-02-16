//
// Created by jervelund on 9/28/17.
//
#include <immintrin.h>
#include <stdio.h>

#include "main.h"

void brackettest(){

    int a = 1;
    printf("%i",a);
    {
        int a = 2;
        printf("%i",a);
        {
            int a = 3;
            printf("%i",a);
        }
        printf("%i",a);
    }
    printf("%i",a);
}

int main() {
    brackettest();


    return 0;
}
