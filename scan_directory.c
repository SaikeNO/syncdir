#include <assert.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include "list.h"
#include "helpers.h"

int scan_directory(const char *dir_path, List *list)
{
    DIR *dir;
    struct dirent *entry;
    char src_path[PATH_MAX + 1];

    dir = opendir(dir_path);
    if (dir == NULL)
    {
        perror("opendir");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        FileState *filestate = (FileState *)malloc(sizeof(FileState));
        snprintf(filestate->filename, PATH_MAX + 1, "%s", entry->d_name);

        snprintf(src_path, PATH_MAX + 1, "%s/%s", dir_path, entry->d_name);

        if (calculate_sha(src_path, filestate->hash) != 0)
        {
            perror("Failed to calculate a hash");
            closedir(dir);
            return -1;
        };

        add_to_list(list, filestate);
    }

    closedir(dir);
    return 0;
}
