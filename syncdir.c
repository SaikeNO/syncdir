#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <time.h>
#include "helpers.h"

void syncdir(const char *src_dir, const char *dest_dir, List *list)
{
  DIR *src_dp, *dest_dp;
  struct dirent *src_entry, *dest_entry;
  struct stat src_stat, dest_stat;
  char src_path[PATH_MAX + 1];
  char dest_path[PATH_MAX + 1];

  src_dp = opendir(src_dir);
  if (src_dp == NULL)
  {
    perror("opendir");
    return;
  }

  dest_dp = opendir(dest_dir);
  if (dest_dp == NULL)
  {
    perror("opendir");
    closedir(src_dp);
    return;
  }

  Node *current_node = list->head;
  while (current_node != NULL)
  {
    snprintf(src_path, PATH_MAX + 1, "%s/%s", src_dir, current_node->data->filename);
    if (lstat(src_path, &src_stat) == -1)
    {
      perror("lstat");
      continue;
    }

    snprintf(dest_path, PATH_MAX + 1, "%s/%s", dest_dir, current_node->data->filename);

    if (lstat(dest_path, &dest_stat) == -1)
    {
      // File doesn't exist in destination directory, copy it
      copy_file(src_path, dest_path);
      printf("Copied %s to %s\n", src_path, dest_path);
    }
    else
    {
      unsigned char sha_result[SHA_DIGEST_LENGTH];
      calculate_sha(src_path, sha_result);
      if (memcmp(current_node->data->hash, sha_result, SHA_DIGEST_LENGTH) != 0)
      {
        copy_file(src_path, dest_path);
        strcpy(current_node->data->hash, sha_result);
        printf("Updated %s to %s\n", src_path, dest_path);
      }
    }

    current_node = current_node->next;
  }

  /* All done. */
  closedir(src_dp);
  closedir(dest_dp);
}