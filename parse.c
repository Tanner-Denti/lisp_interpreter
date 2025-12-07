#include "parse.h"
#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "psi_types.h"
#include "pval.h"
#include "utils.h"

void parse_token_str(char** input, char token_str[]) {
    char* start = *input;

    while (**input != ')' && **input != '(' && **input != ' ' &&
           **input != '\n' && **input != '\t' && **input != '\0') {
        (*input)++;
    }

    size_t len = *input - start;
    strncpy(token_str, start, len);
    token_str[len] = '\0';
}

pval* parse_list(res_array* pval_list, char** input) {
    res_array* list_contents = res_array_new();

    while (**input) {
        while (**input == ' ' || **input == '\n' || **input == '\t') {
            (*input)++;
        }

        if (**input == '\0') {
            res_array_delete(list_contents);
            return new_pval_error(new_pval_string("("), INCOMPLETE_PARSE);
        }

        if (**input == ')') {
            (*input)++;
            return new_pval_list(list_contents);
        }

        char token_str[4096];
        pval* wrapped = NULL;

        if (**input == '(') {
            (*input)++;
            pval* nested = parse_list(pval_list, input);
            if (nested && get_pval_type(nested) == ERROR) {
                res_array_delete(list_contents);
                return nested;
            }
            wrapped = nested;
        } else {
            parse_token_str(input, token_str);

            if (token_str[0] == '\0') {
                continue;
            } else if (strcmp(token_str, "#t") == 0) {
                wrapped = new_pval_boolean(true);
            } else if (strcmp(token_str, "#f") == 0) {
                wrapped = new_pval_boolean(false);
            } else if (token_str[0] == '-' && token_str[1] == '-' &&
                       token_str[2] != '\0') {
                int64_t pval_number;
                bool success = string_to_number(token_str + 2, &pval_number);
                if (!success) {
                    res_array_delete(list_contents);
                    return new_pval_error(new_pval_string(token_str),
                                          INVALID_TOKEN);
                }
                res_array* minus_list = res_array_new();
                res_array_push(minus_list, new_pval_symbol("-"));
                res_array_push(minus_list, new_pval_number(pval_number));
                res_array_push(minus_list, new_pval_number(1));
                wrapped = new_pval_list(minus_list);
            } else if ((token_str[0] == '-' && token_str[1] != '\0' &&
                        isdigit(token_str[1])) ||
                       isdigit(token_str[0])) {
                int64_t pval_number;
                bool success = string_to_number(token_str, &pval_number);
                if (!success) {
                    res_array_delete(list_contents);
                    return new_pval_error(new_pval_string(token_str),
                                          INVALID_TOKEN);
                }
                wrapped = new_pval_number(pval_number);
            } else {
                wrapped = new_pval_symbol(token_str);
            }
        }

        if (wrapped) {
            res_array_push(list_contents, wrapped);
        }
    }

    res_array_delete(list_contents);
    return new_pval_error(new_pval_string("("), INCOMPLETE_PARSE);
}

pval* parse_input(res_array* pval_list, char* input) {
    while (*input) {
        while (*input == ' ' || *input == '\n' || *input == '\t') {
            input++;
        }

        if (*input == '\0')
            break;

        char token_str[4096];
        pval* wrapped = NULL;

        if (*input == '(') {
            input++;
            pval* list_result = parse_list(pval_list, &input);
            if (list_result && get_pval_type(list_result) == ERROR) {
                return list_result;
            }
            wrapped = list_result;
        } else if (*input == ')') {
            return new_pval_error(new_pval_string(")"), INVALID_TOKEN);
        } else {
            parse_token_str(&input, token_str);

            if (token_str[0] == '\0') {
                continue;
            } else if (strcmp(token_str, "#t") == 0) {
                wrapped = new_pval_boolean(true);
            } else if (strcmp(token_str, "#f") == 0) {
                wrapped = new_pval_boolean(false);
            } else if (token_str[0] == '-' && token_str[1] == '-' &&
                       token_str[2] != '\0') {
                int64_t pval_number;
                bool success = string_to_number(token_str + 2, &pval_number);
                if (!success) {
                    return new_pval_error(new_pval_string(token_str),
                                          INVALID_TOKEN);
                }
                res_array* minus_list = res_array_new();
                res_array_push(minus_list, new_pval_symbol("-"));
                res_array_push(minus_list, new_pval_number(pval_number));
                res_array_push(minus_list, new_pval_number(1));
                wrapped = new_pval_list(minus_list);
            } else if ((token_str[0] == '-' && token_str[1] != '\0' &&
                        isdigit(token_str[1])) ||
                       isdigit(token_str[0])) {
                int64_t pval_number;
                bool success = string_to_number(token_str, &pval_number);
                if (!success) {
                    return new_pval_error(new_pval_string(token_str),
                                          INVALID_TOKEN);
                }
                wrapped = new_pval_number(pval_number);
            } else {
                wrapped = new_pval_symbol(token_str);
            }
        }

        if (wrapped) {
            res_array_push(pval_list, wrapped);
        }
    }

    return NULL;
}
