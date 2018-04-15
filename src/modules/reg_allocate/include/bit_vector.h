#ifndef __bit_vector_h
#define __bit_vector_h

//Theory behind the bit vector is from: http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
//Bit vector set operations from: https://en.wikipedia.org/wiki/Bit_array#Basic_operations




int defined;

typedef unsigned *BITVECTOR;

BITVECTOR init_vector();

void set_bit(BITVECTOR bv, int bit);

void clear_bit(BITVECTOR bv, int bit);

int get_bit(BITVECTOR bv, int bit);

BITVECTOR vector_union(BITVECTOR vec1, BITVECTOR vec2);

BITVECTOR vector_difference(BITVECTOR vec1, BITVECTOR vec2);

int vector_compare(BITVECTOR vec1, BITVECTOR vec2);

void vector_print(BITVECTOR bv);

char *vector_as_text(BITVECTOR bv, char *buffer);

int round_up(int v);


#endif