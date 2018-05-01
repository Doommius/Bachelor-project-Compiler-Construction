/**
 * @brief Various helper functions.
 * 
 * @file auxiliary.c
 * @author Morten Jæger, Mark Jervelund & Troels Blicher Petersen
 * @date 2018-03-09
 */

#include <stdlib.h>
#include "auxiliary.h"
#include <libgen.h>
#include <string.h>

int ends_with(char const *str, char const *suffix) {
    size_t l0 = strlen(str);
    size_t l1 = strlen(suffix);

    if (l0 < l1) {
        return 0;
    }

    return strcmp(str + l0 - l1, suffix) == 0;
}

char *get_filename(char const *str, char const *suffix) {
    char *base = basename(str);
    return remove_suffix(base, suffix);
}

char *remove_suffix(char const *str, char const *suffix) {
    char *result = malloc(strlen(str) - strlen(suffix));
    memcpy(result, str, strlen(str)- strlen(suffix));
    return result;
}

char *concat_string(char const *prefix, char const *suffix) {
    char *result = malloc(strlen(prefix) + strlen(suffix) + 1);

    strcpy(result, prefix);
    strcat(result, suffix);

	prefix = result;
	//printf(prefix);
    return result;
}
