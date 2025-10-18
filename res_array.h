#ifndef RES_ARRAY_H
#define RES_ARRAY_H

#include <stdbool.h>

typedef struct res_array res_array;
typedef struct pval pval;

res_array* res_array_new(void);
void res_array_delete(res_array* r);
pval* res_array_get(res_array* r, int idx);
bool res_array_set(res_array* r, int idx, pval* val);
bool res_array_push(res_array* r, pval* elem);
pval* res_array_pop(res_array* r);

#endif
