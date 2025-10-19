#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* allocate_string(char* s) {
    char* out = malloc(strlen(s) + 1);
    assert(out &&
           "Assertion error in allocate_string: memory allocation failure");

    strcpy(out, s);
    return out;
}

char* allocate_string_from_int(int64_t n) {
    char buf[20];
    snprintf(buf, sizeof(buf), "%" PRId64, n);

    return allocate_string(buf);
}

char* allocate_str_formatted_string(char* s, char* inner) {
    size_t len = snprintf(NULL, 0, s, inner) + 1;

    char* out = malloc(len);
    assert(
        out &&
        "Assertion error in allocate_str_formatted_string: Memory allocation "
        "failure");

    snprintf(out, len, s, inner);

    return out;
}
