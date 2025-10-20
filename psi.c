#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "psi_types.h"
#include "pval.h"
#include "res_array.h"

#define MAX_INPUT_LENGTH 4096

pval* evaluate_input(char* input) {
    res_array* pval_list = res_array_new();

    pval* err = parse_input(pval_list, input);

    // res_array_push(pval_list, new_pval_number(10));


    // Parse input into pvals and add them into res_array
    // evaluate items in the res_array until we get out result
    // return the result
}

int main(int argc, char** argv) {
    // REPL
    if (argc == 1) {
        // print "psi>" leaving room for input on the right
        // When the user presses enter, parse and evaluate the input
        // print the output
        // start over
        char input[MAX_INPUT_LENGTH];

        bool quit = false;
        while (!quit) {
            // Read
            printf("psi> ");

            if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {
                printf("\n");
                continue;
            }

            size_t len = strlen(input);
            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
            }

            // Evaluate
            pval* output = evaluate_input(input);
            // Check if output is an error

            // Print

            // Check if output is the quit builtin

            pval_delete(output);
        }

        return 0;
    }

    // File handling
}
