#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <openssl/sha.h>

#define BUF_SIZE 1024

int calculate_sha(const char *file_path, unsigned char *sha_result)
{
    FILE *file = fopen(file_path, "rb");
    if (!file)
    {
        perror("Error opening file");
        return -1;
    }

    SHA_CTX context;
    if (!SHA1_Init(&context))
    {
        perror("Error initializing SHA hash");
        fclose(file);
        return -1;
    }

    unsigned char buffer[BUF_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, BUF_SIZE, file)))
    {
        if (!SHA1_Update(&context, buffer, bytes_read))
        {
            perror("Error updating SHA hash");
            fclose(file);
            return -1;
        }
    }

    if (!SHA1_Final(sha_result, &context))
    {
        perror("Error finalizing SHA hash");
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}
