#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

char* allocate_string(char* s);
char* allocate_string_from_int(int64_t n);
char* allocate_str_formatted_string(char* s, char* inner);
bool string_to_number(char* str, int64_t* res);

#endif
