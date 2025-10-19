#include "pval.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "psi_types.h"
#include "res_array.h"
#include "utils.h"

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

char* pval_to_string(pval* val) {
    assert(val && "Assertion error in pval_to_string: pval cannot be null");
    size_t len;
    char* new_res;

    char* res;
    switch (val->type) {
        case NUMBER:
            res = allocate_string_from_int(val->value.number);
            break;
        case BOOLEAN:
            res = val->value.boolean ? allocate_string("#t")
                                     : allocate_string("#f");
            break;
        case STRING:
            res = allocate_string(val->value.string);
            break;
        case SYMBOL:
            res = allocate_string(val->value.symbol);
            break;
        case LIST:
            res = res_array_to_string(val->value.list);
            break;
        case CELL:
            res = pval_to_string(val->value.cell);

            len = sizeof(char) * (strlen(res) + 9) + sizeof(pval*);
            new_res = realloc(res, len);
            assert(
                new_res &&
                "Assertion error in pval_to_string: memory allocation error");

            snprintf(new_res, len, "$cell{%s}@%p", res, val->value.cell);
            res = new_res;
            break;
        case ERROR:
            res = allocate_string(val->value.error->message);

            len = sizeof(char) * (strlen(res) + 9);
            new_res = realloc(res, len);
            assert(
                new_res &&
                "Assertion error in pval_to_string: memory allocation error");

            snprintf(new_res, len, "$error{%s}", res);
            res = new_res;
            break;
        case FUNCTION:
            if (val->value.function->type == BUILTIN) {
                // Keep an eye on this... I wonder if builtin needs to be a ptr
                // to a function...
                res = allocate_string(
                    val->value.function->definition.builtin.symbol);

                len = sizeof(char) * (strlen(res) + 11);
                new_res = realloc(res, len);
                assert(new_res &&
                       "Assertion error in pval_to_string: memory allocation "
                       "error");

                snprintf(new_res, len, "$builtin{%s}", res);
                res = new_res;
            }
            break;
    }

    return res;
}

void pval_delete(pval* val) {
    assert(val && "Assertion error in pval_delete: pval cannot be null");
    switch (val->type) {
        case LIST:
            res_array_delete(val->value.list);
            break;
        case CELL:
            pval_delete(val->value.cell);
            break;
        case ERROR:
            pval_delete(val->value.error->cause);
            free(val->value.error->message);
            free(val->value.error);
            break;
        case FUNCTION:
            // TODO: Edit this once the new function function is complete
            free(val->value.function);
            break;
        case STRING:
            free(val->value.string);
            break;
        case SYMBOL:
            free(val->value.symbol);
            break;
        default:
            break;
    }

    free(val);
}

pval* new_pval_number(int64_t n) {
    pval* out = malloc(sizeof(pval));
    assert(out &&
           "Assertion error in new_pval_number: memory allocation failure");

    out->type = NUMBER;
    out->value.number = n;

    return out;
}

pval* new_pval_boolean(bool b) {
    pval* out = malloc(sizeof(pval));
    assert(out &&
           "Assertion error in new_pval_boolean: memory allocation failure");

    out->type = BOOLEAN;
    out->value.boolean = b;

    return out;
}

pval* new_pval_string(char* s) {
    pval* out = malloc(sizeof(pval));
    assert(out &&
           "Assertion error in new_pval_string: memory allocation failure");

    out->type = STRING;
    out->value.string = allocate_string(s);

    return out;
}

pval* new_pval_symbol(char* s) {
    pval* out = malloc(sizeof(pval));
    assert(out &&
           "Assertion error in new_pval_symbol: memory allocation failure");

    out->type = SYMBOL;
    out->value.symbol = allocate_string(s);

    return out;
}

pval* new_pval_list(res_array* arr) {
    pval* out = malloc(sizeof(pval));
    assert(out &&
           "Assertion error in new_pval_list: memory allocation failure");

    out->type = LIST;
    out->value.list = arr;

    return out;
}

pval* new_pval_cell(pval* pval_ptr) {
    pval* out = malloc(sizeof(pval));
    assert(out &&
           "Assertion error in new_pval_cell: memory allocation failure");

    out->type = CELL;
    out->value.cell = pval_ptr;

    return out;
}

char* get_error_message(ERROR_TYPE err, pval* cause) {
    char* msg;
    char* pval_str = pval_to_string(cause);
    size_t len;

    switch (err) {
        case DIVISION_BY_ZERO:
            msg = allocate_string("division-by-zero");
            break;
        case INCOMPLETE_PARSE:
            msg = allocate_str_formatted_string("(incomplete-parse \"%s\")",
                                                pval_str);
            break;
        case INVALID_TOKEN:
            msg = allocate_str_formatted_string("(invalid-token \"%s\")",
                                                pval_str);
            break;
        case VALUE_ERROR:
            msg =
                allocate_str_formatted_string("(value-error \"%s\")", pval_str);
            break;
        case TYPE_ERROR:
            // TODO: This is inadequate for later versions, update this to spec.
            msg =
                allocate_str_formatted_string("(type-error \"%s\")", pval_str);
            break;
        case UNBOUND:
            msg = allocate_str_formatted_string("(unbound-error \"%s\")",
                                                pval_str);
            break;
        case ARITY:
            // TODO: This is inadequate for later versions, update this to spec.
            msg =
                allocate_str_formatted_string("(arity-error \"%s\")", pval_str);
            break;
        case INAPPLICABLE_HEAD:
            msg = allocate_string("inapplicable-head");
            break;
        case PROTECTED_SYMBOL:
            msg = allocate_str_formatted_string(
                "(protected-symbol-error \"%s\")", pval_str);
            break;
        case ARGLIST:
            // TODO: This is inadequate (probably) for later versions, update
            // this to spec.
            msg = allocate_str_formatted_string("(arglist-error \"%s\")",
                                                pval_str);
            break;
        default:
            msg = "Unknown Error";
            break;
    }

    free(pval_str);
    return msg;
}

pval* new_pval_error(pval* cause, ERROR_TYPE err) {
    pval* out = malloc(sizeof(pval));
    assert(out &&
           "Assertion error in new_pval_error: memory allocation failure");

    out->type = ERROR;

    pval_error* pval_e = malloc(sizeof(pval_error));
    assert(pval_e &&
           "Assertion error in new_pval_number: partial memory allocation "
           "failure");

    pval_e->type = err;
    pval_e->cause = cause;
    pval_e->message = get_error_message(err, cause);

    out->value.error = pval_e;

    return out;
}

// TODO!
pval* new_pval_function() {
    pval* out = malloc(sizeof(pval));
    assert(out &&
           "Assertion error in new_pval_function: memory allocation failure");

    return out;
}
