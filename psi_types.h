#ifndef PSI_TYPES_H
#define PSI_TYPES_H

typedef enum {
    NUMBER,
    BOOLEAN,
    STRING,
    SYMBOL,
    LIST,
    CELL,
    ERROR,
    FUNCTION,
} PVAL_TYPE;

typedef enum { BUILTIN, LAMBDA } FUNCTION_TYPE;

typedef enum { SAMPLE_FOR_NOW } ERROR_TYPE;

#endif
