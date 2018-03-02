
#include <stdlib.h>
#include "helper.h"

int ends_with(char const *str, char const *suffix) {
    size_t l0 = strlen(str);
    size_t l1 = strlen(suffix);

    if (l0 < l1)
        return 0;

    return strcmp(str + l0 - l1, suffix) == 0;
}