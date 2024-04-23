#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <syslog.h>

#define BUF_SIZE 1024
#define THRESHOLD 1048576 // 1 MB

void copy_file(const char *src_path, const char *dest_path, size_t threshold)
{
    int src_fd, dest_fd;

    src_fd = open(src_path, O_RDONLY);
    if (src_fd == -1)
    {
        perror("open");
        return;
    }

    off_t src_size = lseek(src_fd, 0, SEEK_END);
    if (src_size == -1)
    {
        perror("lseek");
        close(src_fd);
        return;
    }

    /* Small file*/
    if (src_size <= threshold)
    {
        char buffer[BUF_SIZE];
        ssize_t bytes_read, bytes_written;

        lseek(src_fd, 0, SEEK_SET);

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
            close(src_fd);
            close(dest_fd);
            return;
        }

        close(dest_fd);
    }
    else
    {
        /* Large file*/
        syslog(LOG_WARNING, "START COPYING LARGE FILE");
        off_t offset = 0;
        ssize_t bytes_copied;

        dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (dest_fd == -1)
        {
            perror("open");
            close(src_fd);
            return;
        }

        bytes_copied = copy_file_range(src_fd, &offset, dest_fd, &offset, src_size, 0);
        if (bytes_copied == -1)
        {
            perror("copy_file_range");
            close(src_fd);
            close(dest_fd);
            return;
        }
    }

    close(src_fd);
}
