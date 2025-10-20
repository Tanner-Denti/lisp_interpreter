#include "parse.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "psi_types.h"
#include "pval.h"
#include "utils.h"

void parse_token_str(char** input, char token_str[]) {
    // Loop until we find the end, and return that string
    char* start = *input;

    while (**input != ')' && **input != ' ' && **input != '\n' &&
           **input != '\0') {
        (*input)++;
    }

    size_t len = *input - start;

    strncpy(token_str, start, len);
    token_str[len] = '\0';
}

pval* parse_input(res_array* pval_list, char* input) {
    while (*input) {
        while (*input == ' ') {
            input++;
        }

        if (*input == '\0')
            break;

        char token_str[4096];
        pval* wrapped;

        parse_token_str(&input, token_str);

        if (token_str[0] == '\0') {
            input++;
        } else if (token_str[0] == '(') {
            // Parse list
        } else if (token_str[1] && token_str[0] == '-' && token_str[1] == '-') {
            // Check for --number case EXAMPLE:
            // psi> --3
            // 2
            // Store the list:
            // (- 3 1) so that when it comes time for evaluation it will run
            // properly.
        } else if (token_str[0] == '-' ||
                   '0' <= token_str[0] && token_str[1] <= '9') {
            int64_t pval_number;
            bool success = string_to_number(token_str, &pval_number);
            if (!success) {
                return new_pval_error(new_pval_string(token_str),
                                      INVALID_TOKEN);
            }
            wrapped = new_pval_number(pval_number);
        }

        input++;
    }

    return NULL;
}
