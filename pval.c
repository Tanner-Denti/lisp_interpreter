#include "pval.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include "psi_types.h"
#include "res_array.h"


struct pval_function {
    FUNCTION_TYPE type;

    // As a union so it is extesible in later submissions.
    union {
        struct {
            char* symbol;
            pval* (*function)(pval** args, int argc);
        } builtin;

        // Add a second struct here in later phase for user defined functions
    } definition;
};

struct pval_error {
    ERROR_TYPE type;
    char* message;
    pval* cause;
};

struct pval {
    PVAL_TYPE type;

    union {
        int64_t number;
        bool boolean;
        char* string;
        char* symbol;
        res_array* list;
        pval* cell;
        pval_error* error;
        pval_function* function;
    } value;
};

// TODO finish this function. It gets called in res_array_delete
void pval_delete(pval* val) {
    if (val) {
        switch (val->type) {
            case LIST:
                res_array_delete(val->value.list);
                break;
            case CELL:
                pval_delete(val->value.pval);
                break;
            case ERROR:
                pval_delete(val->value.error->cause);
                free(val->value.error->message);
                free(val->value.error);
                break;
            case FUNCTION:
                free(val->value.function);
                break;
            default:
                break;
        }
    }

    free(val);
}

pval* new_pval_number(int64_t n) {
    pval* out = malloc(sizeof(pval));
    assert(out && "Assertion error in new_pval_number: memory allocation failure");

    out->type = NUMBER;
    out->value.number = n;

    return out;
}

pval* new_pval_boolean(bool b) {
    pval* out = malloc(sizeof(pval));
    assert(out && "Assertion error in new_pval_boolean: memory allocation failure");

    out->type = BOOLEAN;
    out->value.boolean = b;

    return out;
}

pval* new_pval_string(char* s) {
    pval* out = malloc(sizeof(pval));
    assert(out && "Assertion error in new_pval_string: memory allocation failure");

    out->type = STRING;
    out->value.string = s;

    return out;
}

pval* new_pval_symbol(char* s) {
    pval* out = malloc(sizeof(pval));
    assert(out && "Assertion error in new_pval_symbol: memory allocation failure");

    out->type = SYMBOL;
    out->value.symbol = s;

    return out;
}

pval* new_pval_list(res_array* arr) {
    pval* out = malloc(sizeof(pval));
    assert(out && "Assertion error in new_pval_list: memory allocation failure");

    out->type = LIST;
    out->value.list = arr;

    return out;
}

pval* new_pval_cell(pval* pval_ptr) {
    pval* out = malloc(sizeof(pval));
    assert(out && "Assertion error in new_pval_cell: memory allocation failure");

    out->type = CELL;
    out->value.cell = pval_ptr;

    return out;
}

// TODO Consider how errors need to be printed, and store the print value in the
// message.
char* get_error_message(ERROR_TYPE err) {
    char* msg;
    switch (err) {
        case SAMPLE_FOR_NOW:
            msg = "...";
            break;
        default:
            msg = "...";
            break;
    }

    char* res = malloc(sizeof(msg));
    assert(out && "Assertion error in get_error_message: memory allocation failure");

    res = msg;
    return res;
}

pval* new_pval_error(pval* cause, ERROR_TYPE err) {
    pval* out = malloc(sizeof(pval));
    assert(out && "Assertion error in new_pval_error: memory allocation failure");

    out->type = ERROR;

    pval_error* pval_e = malloc(sizeof(pval_error));
    assert(pval_e && "Assertion error in new_pval_number: partial memory allocation failure");

    pval_e->type = err;
    pval_e->cause = cause;
    pval_e->message = get_error_message(err);

    return out;
}

// TODO!
pval* new_pval_function() {
    pval* out = malloc(sizeof(pval));
    assert(out && "Assertion error in new_pval_function: memory allocation failure");

    return out;
}
