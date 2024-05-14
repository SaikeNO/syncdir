#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <syslog.h>
#include "../headers/list.h"
#include "../headers/helpers.h"
#include "../headers/copy_file.h"

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
        char absolutePath[PATH_MAX];
        sprintf(absolutePath, "%s/%s", dir_path, entry->d_name);
        if (strcmp(absolutePath, ".") == 0 ||
            strcmp(absolutePath, "..") == 0 ||
            get_file_type(absolutePath) == NULL ||
            strcmp(get_file_type(absolutePath), "regular file") != 0)
            continue;

        FileState *filestate = find_from_list(list, entry->d_name);
        if (filestate == NULL)
        {
            filestate = create_new_file_state(entry->d_name, dir_path);
            if (filestate == NULL)
            {
                closedir(dir);
                return -1;
            }
            add_to_list(list, filestate);
        }
    }

    closedir(dir);
    syslog(LOG_INFO, "Scan directory finished");
    return 0;
}
