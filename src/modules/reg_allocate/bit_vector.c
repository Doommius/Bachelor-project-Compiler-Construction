#include <stdio.h>
#include <stdlib.h>
#include "bit_vector.h"
#include "code.h"
#include "memory.h"

    int defined;

    int B_VECTOR_SIZE = 0;
    int B_VECTOR_TYPE_SIZE = 0;
    int B_VECTOR_BYTES = 0;
    int B_VECTOR_INDECES = 0;

BITVECTOR init_vector(int size){
    if (!defined){
        B_VECTOR_SIZE = round_up(get_temps());
        B_VECTOR_TYPE_SIZE =  sizeof(unsigned);
        B_VECTOR_BYTES = (B_VECTOR_SIZE / 8);
        B_VECTOR_INDECES  = (B_VECTOR_BYTES / B_VECTOR_TYPE_SIZE);
        defined++;
    }
    
    BITVECTOR bv = malloc(B_VECTOR_BYTES);
    
    for (int i = 0; i < B_VECTOR_INDECES; i++){
        bv[i] = 0;
    }
    return bv;
}


void set_bit(BITVECTOR bv, int bit){

    int i;
    int pos;
    unsigned int flag;
    i = bit/32;
    pos = bit%32;
    flag = 1;


    flag = flag << pos;

    bv[i] = bv[i] | flag;
}

void clear_bit(BITVECTOR bv, int bit){

    int i;
    int pos;
    unsigned int flag;
    i = bit/32;
    pos = bit%32;
    flag = 1;

    flag = flag << pos;
    flag = ~flag;

    bv[i] = bv[i] & flag;

}

int get_bit(BITVECTOR bv, int bit){

    int i;
    int pos;
    unsigned int flag;
    i = bit/32;
    pos = bit%32;
    flag = 1;

    flag = flag << pos;

    if (bv[i] & flag){
        return 1;
    } else {
        return 0;
    }

}

//Returns true if the bitvector is empty
int vector_empty(BITVECTOR bv){
    for (int i = 0; i < B_VECTOR_INDECES; i++){
        if (bv[i] != 0){
            return 0;
        }
    }
    return 1;
}

//Returns a new bitvector that is a union of two other bitvectors
BITVECTOR vector_union(BITVECTOR vec1, BITVECTOR vec2){
    BITVECTOR vec3;
    vec3 = init_vector(B_VECTOR_SIZE);
    for (int i = 0; i < B_VECTOR_INDECES; i++){
        vec3[i] = vec1[i] | vec2[i];
    }
    return vec3;
}

//Returns a new bitvector that is the difference of two other bitvectors
BITVECTOR vector_difference(BITVECTOR vec1, BITVECTOR vec2){
    BITVECTOR vec3;
    vec3 = init_vector(B_VECTOR_SIZE);
    for (int i = 0; i < B_VECTOR_INDECES; i++){
        vec3[i] = vec1[i] & (~vec2[i]);
    }
    return vec3;

}

//Returns a new bitvector that is the intersection of two other bitvectors
BITVECTOR vector_intersect(BITVECTOR vec1, BITVECTOR vec2){
    BITVECTOR vec3;
    vec3 = init_vector(B_VECTOR_SIZE);
    for (int i = 0; i < B_VECTOR_INDECES; i++){
        vec3[i] = vec1[i] & vec2[i];
    }
    return vec3;

}

//Returns true if vec1 == vec2
int vector_compare(BITVECTOR vec1, BITVECTOR vec2){
    for (int i = 0; i < B_VECTOR_INDECES; i++){
        if ((vec1[i] ^ vec2[i]) != 0){
            return 0;
        }
    }
    return 1;
}

//Print the indeces of the bits that are set
void vector_print(BITVECTOR bv){
    int w;
    int index;
    int first;
    index = 0;
    first = 1;
    for (int i = 0; i < B_VECTOR_INDECES; i++){
        w = 1;
        
        for (int j = 0; j < B_VECTOR_TYPE_SIZE*8; j++){
            if (bv[i] & w){
                if (first){
                    printf("%d", index);
                    first = 0;
                } else {
                    printf(", %d", index);
                }
            }
            index++;
            w = w << 1;
        }
    }
}


char *vector_as_text(BITVECTOR bv, char *buffer){
    int w;
    int index;
    int first;
    index = 0;
    first = 1;
    for (int i = 0; i < B_VECTOR_INDECES; i++){
        w = 1;
        
        for (int j = 0; j < B_VECTOR_TYPE_SIZE*8; j++){
            if (bv[i] & w){
                if (first){
                    sprintf(buffer, "%d", index);
                    first = 0;
                } else {
                    sprintf(buffer, ", %d", index);
                }
            }
            index++;
            w = w << 1;
        }
    }
    return buffer;
}

//"Rounds up" to a multiple of 32
int round_up(int v){

    int min;
    min = 32;

    while (v > min){
        //We smart
        min = min + 32;
        
    }
    return min;

}
