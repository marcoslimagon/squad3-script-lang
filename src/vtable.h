#ifndef __VTABLE_H__
#define __VTABLE_H__

#include <stdlib.h>
#include <string.h>

#include "sqd3_types.h"

typedef char varname_t[100];

typedef enum {
  NEW_CONTEXT,
  SCOPE_LOCAL,
} scope_t;

typedef struct VTABLE_ENTRY {
  varname_t varname;
  scope_t scope;
  SQD3_OBJECT *ref;
  struct VTABLE_ENTRY *next;
  struct VTABLE_ENTRY *previous;
} VTABLE_ENTRY;

void init_vtable(void);
void init_context(varname_t context_name);
void declare_local_variable(varname_t varname, SQD3_OBJECT *value);
void dispose_local_variables();

#endif
