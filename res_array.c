#include "res_array.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "pval.h"


struct res_array {
    pval** buf;
    size_t size;
    size_t capacity;
};

res_array* res_array_new() {
    res_array* out = malloc(sizeof(res_array));
    assert(out && "Assertion error in res_array_new: memory allocation failure");

    out->size = 0;
    out->capacity = 4;
    out->buf = malloc(out->capacity * sizeof(pval*));

    assert(out->buf && "Assertion error in res_array_new: partial memory allocation failure");

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

pval* res_array_get(res_array* r, int idx) {
    assert(r && "Assertion error in res_array_get: cannot access NULL res_array");

    assert(0 <= idx && idx < r->size && "Assertion error in res_array_get: index out of bounds");
    return r->buf[idx];
}

void res_array_set(res_array* r, int idx, pval* val) {
    assert(r && "Assertion error in res_array_set: cannot access NULL res_array");

    assert(0 <= idx && idx < r->size && "Assertion error in res_array_set: index out of bounds");
    r->buf[idx] = val;
}

void res_array_push(res_array* r, pval* elem) {
    assert(r && "Assertion error in res_array_push: cannot access NULL res_array");

    if (r->size == r->capacity) {
        pval** new_buf = realloc(r->buf, 2 * r->capacity * sizeof(pval*));
        assert(new_buf && "Assertion error in res_array_push: memory reallocation failure");

        r->buf = new_buf;
        r->capacity *= 2;
    }
    r->buf[r->size] = elem;
    r->size += 1;
}

pval* res_array_pop(res_array* r) {
    assert(!r || r->size == 0 && "Assertion error in res_array_pop: res_array cannot be NULL or empty");

    int new_size = r->size - 1;
    if (r->capacity > 4 && r->capacity > (4 * new_size)) {
        pval** new_buf = realloc(r->buf, (r->capacity / 2) * sizeof(pval*));
        assert(new_buf && "Assertion error in res_array_pop: memory reallocation failure");

        r->buf = new_buf;
        r->capacity /= 2;
    }

    r->size = new_size;
    return r->buf[new_size];
}
