#ifndef HELPERS_H
#define HELPERS_H

#include "types.h"

const char *get_file_type(const char *path);
FileState *create_new_file_state(char *fileName, const char *dir_path);

#endif /* HELPERS_H */
