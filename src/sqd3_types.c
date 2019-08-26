#include "sqd3_types.h"

extern SQD3_OBJECT *read_value_from_ref(SQD3_OBJECT *object,
                                        bool stop_execution);
#define NORMALIZE(o)                                                           \
  (o->object_type == T_REF) ? read_value_from_ref(o, false) : o

#define READ_CONTENT(o)                                                        \
  (o->object_type == T_FLOAT) ? read_float_from_object(o)                      \
                              : read_integer_from_object(o)

SQD3_OBJECT *integer_from_long_long(integer value) {
  SQD3_OBJECT *ref = malloc(sizeof(SQD3_OBJECT));

  ref->object_type = T_INTEGER;

  ref->value = malloc(sizeof(integer));
  memcpy(ref->value, &value, sizeof(integer));

  return ref;
}

SQD3_OBJECT *string_from_const_char(const char *value) {
  SQD3_OBJECT *ref = malloc(sizeof(SQD3_OBJECT));

  ref->object_type = T_STRING;

  ref->value = malloc(strlen(value));
  memcpy(ref->value, &value, strlen(value));

  return ref;
}

SQD3_OBJECT *build_ref(varname_t varname) {
  SQD3_OBJECT *ref = malloc(sizeof(SQD3_OBJECT));

  ref->object_type = T_REF;

  SQD3_OBJECT_REF_VALUE *ref_value = malloc(sizeof(SQD3_OBJECT_REF_VALUE));
  memcpy(ref_value->varname, varname, strlen(varname));
  ref_value->ref_type = T_VARIABLE;

  ref->value = (void *)ref_value;

  SQD3_OBJECT *ref_content = read_value_from_ref(ref, false);
  if (ref_content != NULL) {
    free(ref);
    return ref_content;
  }

  return ref;
}

SQD3_OBJECT *build_builtin_function_ref(varname_t varname, void *function_ptr) {
  SQD3_OBJECT *ref = malloc(sizeof(SQD3_OBJECT));

  ref->object_type = T_REF;

  SQD3_OBJECT_REF_VALUE *ref_value = malloc(sizeof(SQD3_OBJECT_REF_VALUE));
  memcpy(ref_value->varname, varname, strlen(varname));
  ref_value->ref_type = T_BUILTIN_FUNCTION;
  ref_value->ptr = function_ptr;

  ref->value = (void *)ref_value;
  return ref;
}

SQD3_OBJECT *float_from_float(float value) {
  SQD3_OBJECT *ref = malloc(sizeof(SQD3_OBJECT));

  ref->object_type = T_FLOAT;

  ref->value = malloc(sizeof(float));
  memcpy(ref->value, &value, sizeof(float));

  return ref;
}

SQD3_OBJECT *boolean_from_char(char value) {
  SQD3_OBJECT *ref = malloc(sizeof(SQD3_OBJECT));

  ref->object_type = T_BOOLEAN;

  ref->value = malloc(sizeof(char));
  memcpy(ref->value, &value, sizeof(char));

  return ref;
}

SQD3_OBJECT *clone_object(SQD3_OBJECT *value) {
  SQD3_OBJECT *cloned = malloc(sizeof(SQD3_OBJECT));
  memcpy(cloned, value, sizeof(SQD3_OBJECT));

  cloned->value = malloc(sizeof(value->value));
  memcpy(cloned->value, value->value, sizeof(value->value));

  if (value->object_type == T_REF) {
    ((SQD3_OBJECT_REF_VALUE *)cloned->value)->ptr =
        ((SQD3_OBJECT_REF_VALUE *)value->value)->ptr;
  }

  return cloned;
}

void to_string(SQD3_OBJECT *value, char *destination) {
  if (value->object_type == T_INTEGER) {
    sprintf(destination, "%lld", read_integer_from_object(value));
  }
  if (value->object_type == T_FLOAT) {
    sprintf(destination, "%f", read_float_from_object(value));
  }
  if (value->object_type == T_BOOLEAN) {
    char boolean = read_boolean_from_object(value);
    if (boolean) {
      sprintf(destination, "true");
    } else {
      sprintf(destination, "false");
    }
  }
  if (value->object_type == T_REF) {
    SQD3_OBJECT_REF_VALUE *ref_value = read_ref_value_from_ref(value);
    if (ref_value->ref_type == T_VARIABLE) {
      to_string(read_value_from_ref(value, true), destination);
    }
  }
}

integer read_integer_from_object(SQD3_OBJECT *object) {
  return *((integer *)object->value);
}

SQD3_OBJECT_REF_VALUE *read_ref_value_from_ref(SQD3_OBJECT *object) {
  return ((SQD3_OBJECT_REF_VALUE *)object->value);
}

const char *read_string_from_object(SQD3_OBJECT *object) {
  return *((const char **)object->value);
}

void *read_function_from_object(SQD3_OBJECT *object) {
  return ((SQD3_OBJECT_REF_VALUE *)object->value)->ptr;
}

float read_float_from_object(SQD3_OBJECT *object) {
  return *((float *)object->value);
}

char read_boolean_from_object(SQD3_OBJECT *object) {
  return *((char *)object->value);
}

SQD3_OBJECT *execute_operator_plus(SQD3_OBJECT *left, SQD3_OBJECT *right) {
  SQD3_OBJECT *left_object = NORMALIZE(left);
  SQD3_OBJECT *right_object = NORMALIZE(right);

  if (left_object->object_type == T_FLOAT ||
      right_object->object_type == T_FLOAT) {

    float a = READ_CONTENT(left_object);
    float b = READ_CONTENT(right_object);

    return float_from_float(a + b);
  }
  return integer_from_long_long(read_integer_from_object(left_object) +
                                read_integer_from_object(right_object));
}

SQD3_OBJECT *execute_operator_multi(SQD3_OBJECT *left, SQD3_OBJECT *right) {
  SQD3_OBJECT *left_object = NORMALIZE(left);
  SQD3_OBJECT *right_object = NORMALIZE(right);

  if (left_object->object_type == T_FLOAT ||
      right_object->object_type == T_FLOAT) {

    float a = READ_CONTENT(left_object);
    float b = READ_CONTENT(right_object);

    return float_from_float(a * b);
  }
  return integer_from_long_long(read_integer_from_object(left) *
                                read_integer_from_object(right));
}

SQD3_OBJECT *execute_operator_division(SQD3_OBJECT *left, SQD3_OBJECT *right) {
  SQD3_OBJECT *left_object = NORMALIZE(left);
  SQD3_OBJECT *right_object = NORMALIZE(right);

  if (left_object->object_type == T_FLOAT ||
      right_object->object_type == T_FLOAT) {

    float a = READ_CONTENT(left_object);
    float b = READ_CONTENT(right_object);

    return float_from_float(a / b);
  }
  return integer_from_long_long(read_integer_from_object(NORMALIZE(left)) /
                                read_integer_from_object(NORMALIZE(right)));
}

SQD3_OBJECT *execute_operator_minus(SQD3_OBJECT *left, SQD3_OBJECT *right) {
  SQD3_OBJECT *left_object = NORMALIZE(left);
  SQD3_OBJECT *right_object = NORMALIZE(right);

  if (left_object->object_type == T_FLOAT ||
      right_object->object_type == T_FLOAT) {

    float a = READ_CONTENT(left_object);
    float b = READ_CONTENT(right_object);

    return float_from_float(a - b);
  }

  return integer_from_long_long(read_integer_from_object(left) -
                                read_integer_from_object(right));
}

SQD3_OBJECT *execute_operator_unary_op(token op, SQD3_OBJECT *operand) {
  if (op == '!')
    return execute_operator_unary_not(operand);
  if (op == '-')
    return execute_operator_unary_minus(operand);
  return operand;
}

SQD3_OBJECT *execute_operator_unary_minus(SQD3_OBJECT *operand) {
  if (operand->object_type == T_FLOAT) {
    return float_from_float(read_float_from_object(operand) * -1);
  }
  if (operand->object_type == T_INTEGER) {
    return integer_from_long_long(read_integer_from_object(operand) * -1);
  }
  char str_value[100];
  to_string(operand, str_value);
  fprintf(stderr, "cannot execute '-' operator for %s", str_value);
  exit(-10);
}

SQD3_OBJECT *execute_operator_unary_not(SQD3_OBJECT *operand) {
  if (operand->object_type == T_BOOLEAN) {
    return boolean_from_char(!read_boolean_from_object(operand));
  }
  char str_value[100];
  to_string(operand, str_value);
  fprintf(stderr, "cannot execute '!' operator for %s", str_value);
  exit(-11);
}

void free_object(SQD3_OBJECT *object) {
  free(object->value);
  free(object);
}
