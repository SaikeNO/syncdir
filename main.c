#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include "syncdir.h"
#include "scan_directory.h"
#include "helpers.h"
#include "list.h"

int main(int argc, char *argv[])
{

  /* Daemon-specific initialization goes here */
  if (argc < 3)
  {
    fprintf(stderr, "Usage: %s <source_dir> <dest_dir>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (strcmp(get_file_type(argv[1]), "directory") != 0)
  {
    printf("%s is not a directory", argv[1]);
    exit(EXIT_FAILURE);
  }

  if (strcmp(get_file_type(argv[2]), "directory") != 0)
  {
    printf("%s is not a directory", argv[2]);
    exit(EXIT_FAILURE);
  }

  List *list = create_list();
  scan_directory(argv[1], list);
  print_list(list);
  syncdir(argv[1], argv[2], list);

  // pid_t pid, sid;

  // pid = fork();
  // if (pid < 0)
  // {
  //   exit(EXIT_FAILURE);
  // }
  // if (pid > 0)
  // {
  //   exit(EXIT_SUCCESS);
  // }

  // umask(0);

  // /* Open any logs here */
  // openlog("mojelogidodemona", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
  // syslog(LOG_INFO, "Program started by User %d", getuid());

  // /* Create a new SID for the child process */
  // sid = setsid();
  // if (sid < 0)
  // {
  //   syslog(LOG_INFO, "Program failed");
  //   exit(EXIT_FAILURE);
  // }

  // if ((chdir("/")) < 0)
  // {
  //   syslog(LOG_INFO, "Program failed");
  //   exit(EXIT_FAILURE);
  // }

  // close(STDIN_FILENO);
  // close(STDOUT_FILENO);
  // close(STDERR_FILENO);

  // /* The Big Loop */
  // while (1)
  // {
  //   /* Do some task here ... */
  //   syncdir(list, argv[2]);

  //   sleep(10); /* wait 10 seconds */
  // }

  syslog(LOG_INFO, "Program stopped");
  exit(EXIT_SUCCESS);
}