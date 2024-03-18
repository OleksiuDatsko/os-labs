#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#define __USE_MISC
#include <dirent.h>
#include <signal.h>

#define BUF_SIZE 256

void process_files(int pfd[2])
{
    close(pfd[1]);
    char filename[BUF_SIZE];
    int bytes_read;

    while ((bytes_read = read(pfd[0], filename, BUF_SIZE)))
    {
        if (bytes_read == -1)
        {
            perror("Error: read");
            exit(EXIT_FAILURE);
        }

        int fd = open(filename, O_RDONLY);

        if (fd == -1)
        {
            perror("Error: open");
            continue;
        }
        off_t file_size = lseek(fd, 0, SEEK_END);
        close(fd);
        printf("%s -> %ld bytes\n",filename, file_size);
    }
    close(pfd[0]);

    exit(EXIT_SUCCESS);
}

void process_dir(const char *dir_name, int pfd[2])
{
    close(pfd[0]);

    DIR *dir = opendir(dir_name);

    if (dir == NULL)
    {
        perror("Error: open dir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            continue;
        }
        write(pfd[1], entry->d_name, strlen(entry->d_name) + 1);
    }
    closedir(dir);
    close(pfd[1]);
}

int main(int argc, char *argv[])
{
    int pfd[2];
    pipe(pfd);

    pid_t pid = fork();

    signal(SIGINT, SIG_IGN); // ігноруємо сигнал переривання

    if (pid == 0) // Child process
    {
        process_files(pfd);
    }
    else if (pid > 0) // Parent process
    {
        process_dir(argv[1], pfd);

        wait(NULL);
    }

    return 0;
}