#ifndef __SQD3_TYPES_H__
#define __SQD3_TYPES_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "consts.h"

typedef enum {
  T_INTEGER,
  T_FLOAT,
  T_REF,
  T_STRING,
  T_BOOLEAN,
} object_t;

typedef enum {
  T_BUILTIN_FUNCTION,
  T_VARIABLE,
} object_ref_t;

typedef struct SQD3_OBJECT {
  object_t object_type;
  void *value;
} SQD3_OBJECT;

typedef struct SQD3_OBJECT_REF_VALUE {
  object_ref_t ref_type;
  void *ptr;
  varname_t varname;
} SQD3_OBJECT_REF_VALUE;

SQD3_OBJECT *clone_object(SQD3_OBJECT *value);
SQD3_OBJECT *integer_from_long_long(integer value);
SQD3_OBJECT *string_from_const_char(const char *value);
SQD3_OBJECT *float_from_float(float value);
SQD3_OBJECT *build_ref(varname_t varname);
SQD3_OBJECT *build_builtin_function_ref(varname_t varname, void *function_ptr);
SQD3_OBJECT *boolean_from_char(char value);
void to_string(SQD3_OBJECT *value, char *destination);

integer read_integer_from_object(SQD3_OBJECT *object);
float read_float_from_object(SQD3_OBJECT *object);
SQD3_OBJECT_REF_VALUE *read_ref_value_from_ref(SQD3_OBJECT *object);
const char *read_string_from_object(SQD3_OBJECT *object);
void *read_function_from_object(SQD3_OBJECT *object);
char read_boolean_from_object(SQD3_OBJECT *object);

SQD3_OBJECT *execute_operator_plus(SQD3_OBJECT *left, SQD3_OBJECT *right);
SQD3_OBJECT *execute_operator_multi(SQD3_OBJECT *left, SQD3_OBJECT *right);
SQD3_OBJECT *execute_operator_division(SQD3_OBJECT *left, SQD3_OBJECT *right);
SQD3_OBJECT *execute_operator_minus(SQD3_OBJECT *left, SQD3_OBJECT *right);

void invert_number_value(SQD3_OBJECT *object);
void free_object(SQD3_OBJECT *object);

#endif
