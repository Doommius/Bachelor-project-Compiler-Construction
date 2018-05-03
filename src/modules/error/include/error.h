#ifndef __error_h
#define __error_h

char* error_msg;

void print_error(char *error, int code, int line);

char* long_error(char *front, char *middle, char *back);


#endif