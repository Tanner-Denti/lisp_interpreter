#include "pval.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include "psi_types.h"
#include "res_array.h"

// TODO: Add print messages to all assertions like this:
//          "Assertion error in <function_name>: reason for the error"

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
                // Leave in case CELL needs this later
                break;
            case ERROR:
                // Leave in case ERROR needs this later
                break;
            case FUNCTION:
                // Leave in case FUNCTION needs this later
                break;
            default:
                break;
        }
    }

    free(val);
}

pval* new_pval_number(int64_t n) {
    pval* out = malloc(sizeof(pval));
    assert(out);

    out->type = NUMBER;
    out->value.number = n;

    return out;
}

pval* new_pval_boolean(bool b) {
    pval* out = malloc(sizeof(pval));
    assert(out);

    out->type = BOOLEAN;
    out->value.boolean = b;

    return out;
}

pval* new_pval_string(char* s) {
    pval* out = malloc(sizeof(pval));
    assert(out);

    out->type = STRING;
    out->value.string = s;

    return out;
}

pval* new_pval_symbol(char* s) {
    pval* out = malloc(sizeof(pval));
    assert(out);

    out->type = SYMBOL;
    out->value.symbol = s;

    return out;
}

pval* new_pval_list(res_array* arr) {
    pval* out = malloc(sizeof(pval));
    assert(out);

    out->type = LIST;
    out->value.list = arr;

    return out;
}

pval* new_pval_cell(pval* pval_ptr) {
    pval* out = malloc(sizeof(pval));
    assert(out);

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
    assert(res);

    res = msg;
    return res;
}

pval* new_pval_error(pval* cause, ERROR_TYPE err) {
    pval* out = malloc(sizeof(pval));
    assert(out);

    out->type = ERROR;

    pval_error* pval_e = malloc(sizeof(pval_error));
    assert(pval_e);

    pval_e->type = err;
    pval_e->cause = cause;
    pval_e->message = get_error_message(err);

    return out;
}

// TODO!
pval* new_pval_function() {
    pval* out = malloc(sizeof(pval));
    assert(out);

    return out;
}
