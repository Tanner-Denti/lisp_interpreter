#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eval.h"
#include "parse.h"
#include "psi_types.h"
#include "pval.h"
#include "res_array.h"

#define MAX_INPUT_LENGTH 4096

pval* evaluate_input(char* input) {
    res_array* pval_list = res_array_new();

    pval* err = parse_input(pval_list, input);

    if (err != NULL) {
        res_array_delete(pval_list);
        return err;
    }

    int num_exprs = res_array_length(pval_list);

    if (num_exprs == 0) {
        res_array_delete(pval_list);
        return new_pval_error(new_pval_string(input), INCOMPLETE_PARSE);
    }

    pval* expr = res_array_get(pval_list, 0);
    pval* result = eval(expr);

    res_array_delete(pval_list);

    return result;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        char input[MAX_INPUT_LENGTH];

        while (true) {
            printf("psi> ");
            fflush(stdout);

            if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {
                printf("\n");
                break;
            }

            size_t len = strlen(input);
            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
            }

            clearerr(stdin);

            if (strlen(input) == 0) {
                continue;
            }

            pval* output = evaluate_input(input);

            if (output == NULL) {
                continue;
            }

            char* output_str = pval_to_string(output);
            printf("%s\n", output_str);
            fflush(stdout);
            free(output_str);

            pval_delete(output);
        }

        return 0;
    }

    return 0;
}
