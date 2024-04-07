#include <assert.h>
#include <sys/stat.h>

/* Return a string that describes the type of the file system entry PATH. */
const char *get_file_type(const char *path)
{
  struct stat st;
  lstat(path, &st);
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
    /* Unexpected. Each entry should be one of the types above. */
    assert(0);
}