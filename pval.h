#ifndef PVAL_H
#define PVAL_H

#include <stdbool.h>

typedef struct res_array res_array;
typedef struct pval pval;
typedef struct pval_function pval_function;
typedef struct pval_error pval_error;

void pval_delete(pval* val);

#endif
