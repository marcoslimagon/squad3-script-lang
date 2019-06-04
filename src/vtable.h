#ifndef __VTABLE_H__
#define __VTABLE_H__

#include <stdlib.h>
#include <string.h>

#include "sqd3_types.h"

typedef char varname_t[100];

typedef enum {
  SCOPE_LOCAL,
} scope_t;

typedef struct VTABLE_ENTRY {
  varname_t varname;
  scope_t scope;
  SQD3_OBJECT *ref;
  struct VTABLE_ENTRY *next;
} VTABLE_ENTRY;

void init_vtable(void);
void declare_local_variable(varname_t varname, SQD3_OBJECT *value);

#endif
