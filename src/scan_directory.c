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
#include <syslog.h>
#include "../headers/list.h"
#include "../headers/helpers.h"

int scan_directory(const char *dir_path, List *list)
{
    syslog(LOG_INFO, "Scan directory started");

    DIR *dir;
    struct dirent *entry;

    dir = opendir(dir_path);
    if (dir == NULL)
    {
        syslog(LOG_ERR, "Open %s directory failed", dir_path);
        perror("opendir");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        FileState *filestate = create_new_file_state(entry->d_name, dir_path);
        add_to_list(list, filestate);
    }

    closedir(dir);
    syslog(LOG_INFO, "Scan directory finished");
    return 0;
}
