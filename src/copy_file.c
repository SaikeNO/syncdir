#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

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
        return; // lub możemy użyć exit(EXIT_FAILURE);
    }

    dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1)
    {
        perror("open");
        close(src_fd);
        return; // lub możemy użyć exit(EXIT_FAILURE);
    }

    while ((bytes_read = read(src_fd, buffer, BUF_SIZE)) > 0)
    {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read)
        {
            perror("write");
            close(src_fd);
            close(dest_fd);
            return; // lub możemy użyć exit(EXIT_FAILURE);
        }
    }

    if (bytes_read == -1)
    {
        perror("read");
        close(src_fd);
        close(dest_fd);
        return; // lub możemy użyć exit(EXIT_FAILURE);
    }

    close(src_fd);
    close(dest_fd);
}
