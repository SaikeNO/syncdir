#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <time.h>
#include <syslog.h>
#include "../headers/helpers.h"
#include "../headers/list.h"

void syncdir(const char *src_dir, const char *dest_dir, List *list)
{
  syslog(LOG_INFO, "Syncdir started");
  struct stat src_stat, dest_stat;
  char src_path[PATH_MAX + 1];
  char dest_path[PATH_MAX + 1];

  Node *current_node = list->head;
  while (current_node != NULL)
  {
    snprintf(src_path, PATH_MAX + 1, "%s/%s", src_dir, current_node->data->filename);
    snprintf(dest_path, PATH_MAX + 1, "%s/%s", dest_dir, current_node->data->filename);

    if (lstat(src_path, &src_stat) == -1)
    {
      // File was removed from source directory
      remove(dest_path);
      Node *node_to_remove = current_node;
      current_node = current_node->next;
      remove_from_list(list, node_to_remove->data->filename);
      syslog(LOG_INFO, "Removed %s\n", dest_path);
      continue;
    }

    if (lstat(dest_path, &dest_stat) == -1)
    {
      // File doesn't exist in destination directory, copy it
      copy_file(src_path, dest_path);
      syslog(LOG_INFO, "Copied %s to %s\n", src_path, dest_path);
    }
    else
    {
      unsigned char sha_result[SHA_DIGEST_LENGTH];
      calculate_sha(src_path, sha_result);
      if (memcmp(current_node->data->hash, sha_result, SHA_DIGEST_LENGTH) != 0)
      {
        copy_file(src_path, dest_path);
        strcpy(current_node->data->hash, sha_result);
        syslog(LOG_INFO, "Updated %s to %s\n", src_path, dest_path);
      }
    }

    current_node = current_node->next;
  }
  print_list(list);
  syslog(LOG_INFO, "Syncdir finished");
}