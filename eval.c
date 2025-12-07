#include "eval.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "psi_types.h"
#include "pval.h"
#include "res_array.h"

pval* builtin_plus(pval** args, int argc) {
    int64_t sum = 0;
    for (int i = 0; i < argc; i++) {
        if (get_pval_type(args[i]) != NUMBER) {
            return new_pval_error(pval_copy(args[i]), TYPE_ERROR);
        }
        sum += get_pval_number(args[i]);
    }
    return new_pval_number(sum);
}

pval* builtin_minus(pval** args, int argc) {
    if (argc < 2) {
        return new_pval_error(new_pval_symbol("-"), ARITY);
    }

    for (int i = 0; i < argc; i++) {
        if (get_pval_type(args[i]) != NUMBER) {
            return new_pval_error(pval_copy(args[i]), TYPE_ERROR);
        }
    }

    int64_t result = get_pval_number(args[0]);
    for (int i = 1; i < argc; i++) {
        result -= get_pval_number(args[i]);
    }

    return new_pval_number(result);
}

pval* builtin_times(pval** args, int argc) {
    int64_t product = 1;
    for (int i = 0; i < argc; i++) {
        if (get_pval_type(args[i]) != NUMBER) {
            return new_pval_error(pval_copy(args[i]), TYPE_ERROR);
        }
        product *= get_pval_number(args[i]);
    }
    return new_pval_number(product);
}

pval* builtin_div(pval** args, int argc) {
    if (argc != 2) {
        return new_pval_error(new_pval_symbol("/"), ARITY);
    }

    if (get_pval_type(args[0]) != NUMBER || get_pval_type(args[1]) != NUMBER) {
        int idx = (get_pval_type(args[0]) != NUMBER) ? 0 : 1;
        return new_pval_error(args[idx], TYPE_ERROR);
    }

    int64_t divisor = get_pval_number(args[1]);
    if (divisor == 0) {
        return new_pval_error(new_pval_symbol("division-by-zero"),
                              DIVISION_BY_ZERO);
    }

    int64_t dividend = get_pval_number(args[0]);
    int64_t result = dividend / divisor;

    if ((dividend < 0) != (divisor < 0) && dividend % divisor != 0) {
        result--;
    }

    return new_pval_number(result);
}

pval* builtin_equals(pval** args, int argc) {
    if (argc == 0) {
        return new_pval_boolean(true);
    }

    for (int i = 1; i < argc; i++) {
        if (!pval_equals(args[0], args[i])) {
            return new_pval_boolean(false);
        }
    }

    return new_pval_boolean(true);
}

pval* builtin_quit(pval** args, int argc) {
    if (argc != 0) {
        return new_pval_error(new_pval_symbol("quit"), ARITY);
    }
    exit(0);
    return NULL;
}

pval* apply_builtin(char* func_name, pval** args, int argc) {
    if (strcmp(func_name, "+") == 0) {
        return builtin_plus(args, argc);
    } else if (strcmp(func_name, "-") == 0) {
        return builtin_minus(args, argc);
    } else if (strcmp(func_name, "*") == 0) {
        return builtin_times(args, argc);
    } else if (strcmp(func_name, "/") == 0) {
        return builtin_div(args, argc);
    } else if (strcmp(func_name, "=") == 0) {
        return builtin_equals(args, argc);
    } else if (strcmp(func_name, "quit") == 0) {
        return builtin_quit(args, argc);
    } else {
        return new_pval_error(new_pval_symbol(func_name), UNBOUND);
    }
}

pval* eval(pval* expr) {
    if (!expr) {
        return NULL;
    }

    PVAL_TYPE type = get_pval_type(expr);

    if (type == NUMBER || type == BOOLEAN || type == STRING || type == ERROR) {
        return pval_copy(expr);
    }

    if (type == SYMBOL) {
        char* sym = get_pval_symbol(expr);
        if (strcmp(sym, "+") == 0 || strcmp(sym, "-") == 0 ||
            strcmp(sym, "*") == 0 || strcmp(sym, "/") == 0 ||
            strcmp(sym, "=") == 0 || strcmp(sym, "quit") == 0) {
            return pval_copy(expr);
        }
        return new_pval_error(new_pval_symbol(sym), UNBOUND);
    }

    if (type == LIST) {
        res_array* list = get_pval_list(expr);
        int len = res_array_length(list);

        if (len == 0) {
            return new_pval_list(res_array_new());
        }

        pval** evaluated = malloc(len * sizeof(pval*));
        assert(evaluated && "Memory allocation failed in eval");

        for (int i = 0; i < len; i++) {
            pval* sub = res_array_get(list, i);
            evaluated[i] = eval(sub);

            if (evaluated[i] && get_pval_type(evaluated[i]) == ERROR) {
                for (int j = 0; j <= i; j++) {
                    if (j < i) {
                        pval_delete(evaluated[j]);
                    }
                }
                pval* error = evaluated[i];
                free(evaluated);
                return error;
            }
        }

        if (get_pval_type(evaluated[0]) != SYMBOL) {
            for (int i = 0; i < len; i++) {
                pval_delete(evaluated[i]);
            }
            free(evaluated);
            return new_pval_error(new_pval_symbol("inapplicable-head"),
                                  INAPPLICABLE_HEAD);
        }

        char* func_name = get_pval_symbol(evaluated[0]);

        pval* result = apply_builtin(func_name, evaluated + 1, len - 1);

        for (int i = 0; i < len; i++) {
            pval_delete(evaluated[i]);
        }
        free(evaluated);

        return result;
    }

    return new_pval_error(new_pval_symbol("unknown-type"), VALUE_ERROR);
}
