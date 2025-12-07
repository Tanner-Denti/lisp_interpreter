#ifndef PVAL_H
#define PVAL_H

#include <stdbool.h>
#include <stdint.h>
#include "psi_types.h"

typedef struct res_array res_array;
typedef struct pval pval;
typedef struct pval_function pval_function;
typedef struct pval_error pval_error;

void pval_delete(pval* val);
char* pval_to_string(pval* val);
pval* new_pval_number(int64_t n);
pval* new_pval_boolean(bool b);
pval* new_pval_string(char* s);
pval* new_pval_symbol(char* s);
pval* new_pval_list(res_array* arr);
pval* new_pval_cell(pval* pval_ptr);
pval* new_pval_error(pval* cause, ERROR_TYPE err);
pval* new_pval_function(void);

char* get_error_message(ERROR_TYPE err, pval* cause);
PVAL_TYPE get_pval_type(pval* v);
int64_t get_pval_number(pval* v);
bool get_pval_boolean(pval* v);
char* get_pval_symbol(pval* v);
char* get_pval_string(pval* v);
res_array* get_pval_list(pval* v);

pval* pval_copy(pval* val);
bool pval_equals(pval* a, pval* b);

#endif
