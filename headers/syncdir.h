#ifndef SYNCDIR_H
#define SYNCDIR_H

#include "types.h"

void syncdir(const char *src_dir, const char *dest_dir, List *list, size_t threshold);

#endif /* SYNCDIR_H */
