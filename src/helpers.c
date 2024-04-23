#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../headers/list.h"
#include "../headers/calculate_sha.h"

FileState *create_new_file_state(char *fileName, const char *dir_path)
{
  char src_path[PATH_MAX + 1];
  FileState *filestate = (FileState *)malloc(sizeof(FileState));
  if (filestate == NULL)
  {
    perror("malloc");
    return NULL;
  }

  snprintf(filestate->filename, PATH_MAX + 1, "%s", fileName);

  snprintf(src_path, PATH_MAX + 1, "%s/%s", dir_path, fileName);
  if (calculate_sha(src_path, filestate->hash) == -1)
  {
    free(filestate);
    return NULL;
  }

  return filestate;
}

/* Return a string that describes the type of the file system entry PATH. */
const char *get_file_type(const char *path)
{
  struct stat st;
  if (lstat(path, &st) == -1)
  {
    perror("lstat");
    return NULL;
  }

  if (S_ISLNK(st.st_mode))
    return "symbolic link";
  else if (S_ISDIR(st.st_mode))
    return "directory";
  else if (S_ISCHR(st.st_mode))
    return "character device";
  else if (S_ISBLK(st.st_mode))
    return "block device";
  else if (S_ISFIFO(st.st_mode))
    return "fifo";
  else if (S_ISSOCK(st.st_mode))
    return "socket";
  else if (S_ISREG(st.st_mode))
    return "regular file";
  else
  {
    fprintf(stderr, "Unexpected file type.\n");
    return NULL;
  }
}
