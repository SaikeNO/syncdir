#ifndef HEADERS
#define HEADERS
#include "types.h"

const char *get_file_type(const char *path);
void copy_file(const char *src_path, const char *dest_path);
FileState *create_new_file_state(char *fileName, const char *dir_path);
int calculate_sha(const char *file_path, unsigned char *sha_result);
#endif
