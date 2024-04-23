#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include "../headers/syncdir.h"
#include "../headers/scan_directory.h"
#include "../headers/helpers.h"
#include "../headers/list.h"

int main(int argc, char *argv[])
{
  /* First error checking */
  if (argc < 3)
  {
    fprintf(stderr, "Usage: %s <source_dir> <dest_dir>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char cwd[PATH_MAX];
  getcwd(cwd, sizeof(cwd));
  char src_dir[PATH_MAX];
  char dest_dir[PATH_MAX];
  int sleep_time = (argc >= 4) ? atoi(argv[3]) : 300;     // Default sleep time is 5 minutes
  size_t threshold = argc == 5 ? atoi(argv[4]) : 1048576; // Default threshold is 1 MB

  /* Change passed directory path to absolute path */
  if (argv[1][0] != '/')
  {
    sprintf(src_dir, "%s/%s", cwd, argv[1]);
  }
  else
  {
    sprintf(src_dir, "%s", argv[1]);
  }

  if (argv[2][0] != '/')
  {
    sprintf(dest_dir, "%s/%s", cwd, argv[2]);
  }
  else
  {
    sprintf(dest_dir, "%s", argv[2]);
  }

  /* Second error checking */
  if (strcmp(get_file_type(src_dir), "directory") != 0)
  {
    fprintf(stderr, "%s is not a directory", src_dir);
    exit(EXIT_FAILURE);
  }

  if (strcmp(get_file_type(dest_dir), "directory") != 0)
  {
    fprintf(stderr, "%s is not a directory", dest_dir);
    exit(EXIT_FAILURE);
  }

  printf("Copying from %s\n", src_dir);
  printf("to %s\n", dest_dir);

  /* Open logs */
  openlog("syncdir_daemon", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
  syslog(LOG_INFO, "Syncdir daemon started by User %d", getuid());

  pid_t pid, sid;
  pid = fork();
  if (pid < 0)
  {
    syslog(LOG_ERR, "Failed to fork process");
    exit(EXIT_FAILURE);
  }
  if (pid > 0)
  {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0)
  {
    syslog(LOG_ERR, "Failed to set new session");
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0)
  {
    syslog(LOG_ERR, "Failed to change working directory");
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  List *list = create_list();
  if (list == NULL)
  {
    syslog(LOG_ERR, "Failed to create list");
    exit(EXIT_FAILURE);
  }

  while (1)
  {
    if (scan_directory(src_dir, list) == -1)
    {
      syslog(LOG_ERR, "Failed to scan directory");
      exit(EXIT_FAILURE);
    }
    syncdir(src_dir, dest_dir, list, threshold);
    sleep(sleep_time);
  }

  syslog(LOG_INFO, "Syncdir daemon stopped");
  closelog();

  exit(EXIT_SUCCESS);
}
