#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include "syncdir.h"

int main(int argc, char *argv[])
{
  pid_t pid, sid;

  pid = fork();
  if (pid < 0)
  {
    exit(EXIT_FAILURE);
  }
  if (pid > 0)
  {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  /* Open any logs here */
  openlog("mojelogidodemona", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
  syslog(LOG_INFO, "Program started by User %d", getuid());

  /* Create a new SID for the child process */
  sid = setsid();
  if (sid < 0)
  {
    syslog(LOG_INFO, "Program failed");
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0)
  {
    syslog(LOG_INFO, "Program failed");
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  /* Daemon-specific initialization goes here */

  /* The Big Loop */
  while (1)
  {
    /* Do some task here ... */
    syncdir(argc, argv);

    sleep(30); /* wait 30 seconds */
  }

  syslog(LOG_INFO, "Program stopped");
  exit(EXIT_SUCCESS);
}