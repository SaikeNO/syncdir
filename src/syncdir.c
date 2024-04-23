#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <syslog.h>
#include "../headers/list.h"
#include "../headers/helpers.h"
#include "../headers/copy_file.h"
#include "../headers/calculate_sha.h"

void syncdir(const char *src_dir, const char *dest_dir, List *list, size_t threshold)
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
      copy_file(src_path, dest_path, threshold);
      syslog(LOG_INFO, "Copied %s to %s\n", src_path, dest_path);
    }
    else
    {
      unsigned char sha_result[SHA_DIGEST_LENGTH];
      if (calculate_sha(src_path, sha_result) == -1)
      {
        current_node = current_node->next;
        continue;
      }

      if (memcmp(current_node->data->hash, sha_result, SHA_DIGEST_LENGTH) != 0)
      {
        copy_file(src_path, dest_path, threshold);
        memcpy(current_node->data->hash, sha_result, SHA_DIGEST_LENGTH);
        syslog(LOG_INFO, "Updated %s to %s\n", src_path, dest_path);
      }
    }

    current_node = current_node->next;
  }
  syslog(LOG_INFO, "Syncdir finished");
}
