#ifndef TYPES_H
#define TYPES_H

#include <openssl/sha.h>

#define PATH_MAX 4096

typedef struct FileState
{
    char filename[PATH_MAX + 1];
    unsigned char hash[SHA_DIGEST_LENGTH];
} FileState;

typedef struct Node
{
    FileState *data;
    struct Node *next;
} Node;

typedef struct List
{
    int size;
    Node *head;
} List;

#endif /* TYPES_H */
