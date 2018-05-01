#ifndef __auxiliary_h
#define __auxiliary_h
int ends_with(char const *str, char const *suffix);

char* get_filename(char const *str, char const *suffix);

char* remove_suffix(char const *str, char const *suffix);

char* concat_string(char const *prefix, char const *suffix);

#endif