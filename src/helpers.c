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

#define BUF_SIZE 1024

void copy_file(const char *src_path, const char *dest_path)
{
  int src_fd, dest_fd;
  ssize_t bytes_read, bytes_written;
  char buffer[BUF_SIZE];

  src_fd = open(src_path, O_RDONLY);
  if (src_fd == -1)
  {
    perror("open");
    return;
  }

  dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (dest_fd == -1)
  {
    perror("open");
    close(src_fd);
    return;
  }

  while ((bytes_read = read(src_fd, buffer, BUF_SIZE)) > 0)
  {
    bytes_written = write(dest_fd, buffer, bytes_read);
    if (bytes_written != bytes_read)
    {
      perror("write");
      close(src_fd);
      close(dest_fd);
      return;
    }
  }

  if (bytes_read == -1)
  {
    perror("read");
  }

  close(src_fd);
  close(dest_fd);
}

// Function to calculate SHA hash for a file
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

FileState *create_new_file_state(char *fileName, const char *dir_path)
{
  char src_path[PATH_MAX + 1];
  FileState *filestate = (FileState *)malloc(sizeof(FileState));
  snprintf(filestate->filename, PATH_MAX + 1, "%s", fileName);

  snprintf(src_path, PATH_MAX + 1, "%s/%s", dir_path, fileName);
  calculate_sha(src_path, filestate->hash);

  return filestate;
}

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