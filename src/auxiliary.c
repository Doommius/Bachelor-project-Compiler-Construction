/**
 * @brief Various helper functions.
 * 
 * @file auxiliary.c
 * @author Morten Jæger, Mark Jervelund & Troels Blicher Petersen
 * @date 2018-03-09
 */

#include <stdlib.h>
#include "auxiliary.h"

int ends_with(char const *str, char const *suffix) {
    size_t l0 = strlen(str);
    size_t l1 = strlen(suffix);

    if (l0 < l1) {
		return 0;
	}

    return strcmp(str + l0 - l1, suffix) == 0;
}