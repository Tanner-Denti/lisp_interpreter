#include "res_array.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "pval.h"

// TODO replace memory allocation if statement guards with assert statements.

struct res_array {
    pval** buf;
    size_t size;
    size_t capacity;
};

res_array* res_array_new() {
    res_array* out = malloc(sizeof(res_array));
    assert(out);

    out->size = 0;
    out->capacity = 4;
    out->buf = malloc(out->capacity * sizeof(pval*));
    assert(out->buf);

    return out;
}

void res_array_delete(res_array* r) {
    if (r) {
        for (size_t i = 0; i < r->size; i++) {
            pval_delete(r->buf[i]);
        }
        free(r->buf);
    }
    free(r);
}

bool res_array_get(res_array* r, int idx, pval** rtn) {
    assert(r && "Assertion error: cannot access NULL res_array");

    if (rtn && 0 <= idx && idx < r->size) {
        *rtn = r->buf[idx];
        return true;
    } else
        return false;
}

bool res_array_set(res_array* r, int idx, pval* val) {
    if (!r)
        return false;
    if (0 <= idx && idx < r->size) {
        r->buf[idx] = val;
        return true;
    } else
        return false;
}

bool res_array_push(res_array* r, pval* elem) {
    if (!r)
        return false;
    if (r->size == r->capacity) {
        pval** new_buf = realloc(r->buf, 2 * r->capacity * sizeof(pval*));
        if (!new_buf)
            return false;
        r->buf = new_buf;
        r->capacity *= 2;
    }
    r->buf[r->size] = elem;
    r->size += 1;
    return true;
}

bool res_array_pop(res_array* r, pval** elem) {
    if (!r || r->size == 0)
        return false;
    int new_size = r->size - 1;
    if (r->capacity > 4 && r->capacity > (4 * new_size)) {
        pval** new_buf = realloc(r->buf, (r->capacity / 2) * sizeof(pval*));
        if (!new_buf)
            return false;
        else {
            r->buf = new_buf;
            r->capacity /= 2;
        }
    }
    if (elem)
        *elem = r->buf[new_size];
    r->size = new_size;
    return true;
}
