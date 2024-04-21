
#ifndef TYPES
#define TYPES

#include <linux/limits.h>
#include <openssl/sha.h>

typedef struct fileState
{
    char filename[PATH_MAX + 1];
    unsigned char hash[SHA_DIGEST_LENGTH];
} FileState;

typedef struct node
{
    FileState *data;
    struct node *next;
} Node;

typedef struct list
{
    int size;
    Node *head;
} List;
#endif
