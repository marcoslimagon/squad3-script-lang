#include "vtable.h"

VTABLE_ENTRY *first_entry;

VTABLE_ENTRY *get_last_entry(VTABLE_ENTRY *entry) {
  if (entry->next != NULL) {
    return get_last_entry(entry->next);
  }
  return entry;
}

void init_vtable(void) {
  first_entry = malloc(sizeof(VTABLE_ENTRY));
  first_entry->varname[0] = '\0';
  first_entry->ref = NULL;
  first_entry->next = NULL;
  first_entry->previous = NULL;
  first_entry->scope = NEW_CONTEXT;
}

void declare_local_variable(varname_t varname, SQD3_OBJECT *value) {
  VTABLE_ENTRY *last_entry = get_last_entry(first_entry);

  VTABLE_ENTRY *entry = malloc(sizeof(VTABLE_ENTRY));
  strncpy(entry->varname, varname, sizeof(varname_t));
  entry->ref = value;
  entry->next = NULL;
  entry->previous = last_entry;
  entry->scope = SCOPE_LOCAL;

  last_entry->next = entry;
}

void dispose_local_variables() {
  VTABLE_ENTRY *last_entry = get_last_entry(first_entry);

  while (last_entry->scope == SCOPE_LOCAL) {
    last_entry = last_entry->previous;
    free(last_entry->next);
    last_entry->next = NULL;
  }
}
