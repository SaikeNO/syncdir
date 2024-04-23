#ifndef CALCULATE_SHA_H
#define CALCULATE_SHA_H

#include <openssl/sha.h>

int calculate_sha(const char *file_path, unsigned char *sha_result);

#endif /* CALCULATE_SHA_H */
