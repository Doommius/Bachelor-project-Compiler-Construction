//
// Created by jervelund on 9/28/17.
//
#include <immintrin.h>
#include <stdio.h>


void brackettest(){

    int a = 1;
    printf("%i \n",a);
    {
        int a = 2;
        printf("%i\n",a);
        {
            int a = 3;
            printf("%i\n",a);
        }
        printf("%i\n",a);
    }
    printf("%i\n",a);
}

int main() {
    brackettest();


    return 0;
}
