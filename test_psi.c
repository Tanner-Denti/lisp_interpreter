#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "psi_types.h"
#include "pval.h"
#include "res_array.h"

// Helper function to print test results
void print_test(const char* test_name, bool passed) {
    printf("[%s] %s\n", passed ? "PASS" : "FAIL", test_name);
}

// Test res_array functions
void test_res_array_new() {
    res_array* arr = res_array_new();
    print_test("res_array_new", arr != NULL);
    res_array_delete(arr);
}

void test_res_array_push_and_get() {
    res_array* arr = res_array_new();

    pval* val1 = new_pval_number(42);
    pval* val2 = new_pval_number(99);
    pval* val3 = new_pval_boolean(true);

    res_array_push(arr, val1);
    res_array_push(arr, val2);
    res_array_push(arr, val3);

    pval* retrieved1 = res_array_get(arr, 0);
    pval* retrieved2 = res_array_get(arr, 1);
    pval* retrieved3 = res_array_get(arr, 2);

    bool test_passed =
        (retrieved1 == val1) && (retrieved2 == val2) && (retrieved3 == val3);

    print_test("res_array_push_and_get", test_passed);

    res_array_delete(arr);
}

void test_res_array_set() {
    res_array* arr = res_array_new();

    pval* val1 = new_pval_number(10);
    pval* val2 = new_pval_number(20);
    pval* val3 = new_pval_number(30);

    res_array_push(arr, val1);
    res_array_push(arr, val2);

    // Delete old value before replacing
    pval_delete(val1);
    res_array_set(arr, 0, val3);

    pval* retrieved = res_array_get(arr, 0);
    bool test_passed = (retrieved == val3);

    print_test("res_array_set", test_passed);

    res_array_delete(arr);
}

void test_res_array_pop() {
    res_array* arr = res_array_new();

    pval* val1 = new_pval_number(1);
    pval* val2 = new_pval_number(2);
    pval* val3 = new_pval_number(3);

    res_array_push(arr, val1);
    res_array_push(arr, val2);
    res_array_push(arr, val3);

    pval* popped = res_array_pop(arr);
    bool test_passed = (popped == val3);

    print_test("res_array_pop", test_passed);

    pval_delete(popped);
    res_array_delete(arr);
}

void test_res_array_to_string() {
    res_array* arr = res_array_new();

    res_array_push(arr, new_pval_number(1));
    res_array_push(arr, new_pval_number(2));
    res_array_push(arr, new_pval_number(3));

    char* str = res_array_to_string(arr);
    bool test_passed = strcmp(str, "(1 2 3)") == 0;

    print_test("res_array_to_string", test_passed);

    free(str);
    res_array_delete(arr);
}

void test_res_array_empty_to_string() {
    res_array* arr = res_array_new();

    char* str = res_array_to_string(arr);
    bool test_passed = strcmp(str, "()") == 0;

    print_test("res_array_empty_to_string", test_passed);

    free(str);
    res_array_delete(arr);
}

// Test pval functions
void test_new_pval_number() {
    pval* val = new_pval_number(42);
    char* str = pval_to_string(val);
    bool test_passed = strcmp(str, "42") == 0;

    print_test("new_pval_number", test_passed);

    free(str);
    pval_delete(val);
}

void test_new_pval_boolean_true() {
    pval* val = new_pval_boolean(true);
    char* str = pval_to_string(val);
    bool test_passed = strcmp(str, "#t") == 0;

    print_test("new_pval_boolean_true", test_passed);

    free(str);
    pval_delete(val);
}

void test_new_pval_boolean_false() {
    pval* val = new_pval_boolean(false);
    char* str = pval_to_string(val);
    bool test_passed = strcmp(str, "#f") == 0;

    print_test("new_pval_boolean_false", test_passed);

    free(str);
    pval_delete(val);
}

void test_new_pval_string() {
    pval* val = new_pval_string("hello");
    char* str = pval_to_string(val);
    bool test_passed = strcmp(str, "hello") == 0;

    print_test("new_pval_string", test_passed);

    free(str);
    pval_delete(val);
}

void test_new_pval_symbol() {
    pval* val = new_pval_symbol("+");
    char* str = pval_to_string(val);
    bool test_passed = strcmp(str, "+") == 0;

    print_test("new_pval_symbol", test_passed);

    free(str);
    pval_delete(val);
}

void test_new_pval_list() {
    res_array* arr = res_array_new();
    res_array_push(arr, new_pval_number(1));
    res_array_push(arr, new_pval_number(2));
    res_array_push(arr, new_pval_number(3));

    pval* val = new_pval_list(arr);
    char* str = pval_to_string(val);
    bool test_passed = strcmp(str, "(1 2 3)") == 0;

    print_test("new_pval_list", test_passed);

    free(str);
    pval_delete(val);
}

void test_new_pval_list_empty() {
    res_array* arr = res_array_new();
    pval* val = new_pval_list(arr);
    char* str = pval_to_string(val);
    bool test_passed = strcmp(str, "()") == 0;

    print_test("new_pval_list_empty", test_passed);

    free(str);
    pval_delete(val);
}

void test_new_pval_list_nested() {
    res_array* inner = res_array_new();
    res_array_push(inner, new_pval_number(1));
    res_array_push(inner, new_pval_number(2));

    res_array* outer = res_array_new();
    res_array_push(outer, new_pval_number(0));
    res_array_push(outer, new_pval_list(inner));
    res_array_push(outer, new_pval_number(3));

    pval* val = new_pval_list(outer);
    char* str = pval_to_string(val);
    bool test_passed = strcmp(str, "(0 (1 2) 3)") == 0;

    printf("NESTED: %s\n", str);
    print_test("new_pval_list_nested", test_passed);

    free(str);
    pval_delete(val);
}

void test_new_pval_cell() {
    pval* inner = new_pval_number(8);
    pval* val = new_pval_cell(inner);
    char* str = pval_to_string(val);

    // Check that it contains "$cell{8}@" somewhere
    bool test_passed = strstr(str, "$cell{8}@") != NULL;

    printf("CELL: %s\n", str);
    print_test("new_pval_cell", test_passed);

    free(str);
    pval_delete(val);
}

void test_new_pval_error_division_by_zero() {
    pval* cause = new_pval_symbol("division-by-zero");
    pval* val = new_pval_error(cause, DIVISION_BY_ZERO);
    char* str = pval_to_string(val);

    bool test_passed = strcmp(str, "$error{division-by-zero}") == 0;

    printf("ERROR_DBZ: %s\n", str);
    print_test("new_pval_error_division_by_zero", test_passed);

    free(str);
    pval_delete(val);
}

void test_new_pval_error_type_error() {
    pval* cause = new_pval_string("some cause");
    pval* val = new_pval_error(cause, TYPE_ERROR);
    char* str = pval_to_string(val);

    bool test_passed =
        strstr(str, "$error{") != NULL && strstr(str, "type-error") != NULL;

    printf("ERROR_TE: %s\n", str);
    print_test("new_pval_error_type_error", test_passed);

    free(str);
    pval_delete(val);
}

void test_new_pval_error_unbound() {
    pval* cause = new_pval_symbol("x");
    pval* val = new_pval_error(cause, UNBOUND);
    char* str = pval_to_string(val);

    bool test_passed =
        strstr(str, "$error{") != NULL && strstr(str, "unbound-error") != NULL;

    printf("ERROR_UE: %s\n", str);
    print_test("new_pval_error_unbound", test_passed);

    free(str);
    pval_delete(val);
}

void test_pval_to_string_complex() {
    // Create: (+ 1 (* 2 3))
    res_array* inner = res_array_new();
    res_array_push(inner, new_pval_symbol("*"));
    res_array_push(inner, new_pval_number(2));
    res_array_push(inner, new_pval_number(3));

    res_array* outer = res_array_new();
    res_array_push(outer, new_pval_symbol("+"));
    res_array_push(outer, new_pval_number(1));
    res_array_push(outer, new_pval_list(inner));

    pval* val = new_pval_list(outer);
    char* str = pval_to_string(val);
    bool test_passed = strcmp(str, "(+ 1 (* 2 3))") == 0;

    printf("COMPLEX: %s\n", str);
    print_test("pval_to_string_complex", test_passed);

    free(str);
    pval_delete(val);
}

void test_get_error_message() {
    pval* cause = new_pval_number(42);
    char* msg = get_error_message(DIVISION_BY_ZERO, cause);

    bool test_passed = strcmp(msg, "division-by-zero") == 0;

    print_test("get_error_message_division_by_zero", test_passed);

    free(msg);
    pval_delete(cause);
}

int main() {
    printf("=== Testing res_array functions ===\n");
    test_res_array_new();
    test_res_array_push_and_get();
    test_res_array_set();
    test_res_array_pop();
    test_res_array_to_string();
    test_res_array_empty_to_string();

    printf("\n=== Testing pval functions ===\n");
    test_new_pval_number();
    test_new_pval_boolean_true();
    test_new_pval_boolean_false();
    test_new_pval_string();
    test_new_pval_symbol();
    test_new_pval_list();
    test_new_pval_list_empty();
    test_new_pval_list_nested();
    test_new_pval_cell();
    test_new_pval_error_division_by_zero();
    test_new_pval_error_type_error();
    test_new_pval_error_unbound();
    test_pval_to_string_complex();
    test_get_error_message();

    printf("\n=== All tests completed ===\n");

    return 0;
}
