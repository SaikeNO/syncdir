#ifndef LIST
#define LIST

#include "types.h"

List *create_list();
void add_to_list(List *list, FileState *data);
List *remove_from_list(List *list, const char *filename);
Node *find_from_list(List *list, const char *filename);
void free_list(List *list);
void print_list(List *list);
#endif