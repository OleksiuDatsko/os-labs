#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

#define BUF_SIZE 256
#define MAX_PATH_LENGTH 4096

void process_files(const char *dir_name, int pfd[2])
{
    close(pfd[1]);
    signal(SIGINT, SIG_IGN);

    char filename[BUF_SIZE];
    char path[MAX_PATH_LENGTH];

    ssize_t bytes_read;

    while ((bytes_read = read(pfd[0], filename, BUF_SIZE)))
    {
        sprintf(path, "%s/%s", dir_name, filename);
        if (bytes_read == -1)
        {
            perror("Error: read");
            exit(EXIT_FAILURE);
        }

        int fd = open(path, O_RDONLY);

        if (fd == -1)
        {
            perror("Error: open");
            continue;
        }
        off_t file_size = lseek(fd, 0, SEEK_END);
        printf("%s\t%ld bytes\n", filename, file_size);
        close(fd);
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
        if (entry->d_type == DT_REG)
        {
            write(pfd[1], entry->d_name , BUF_SIZE);
        }
    }
    closedir(dir);
    close(pfd[1]);

    wait(NULL);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    int pfd[2];
    pipe(pfd);

    pid_t pid = fork();
    signal(SIGINT, SIG_IGN); // ігноруємо сигнал переривання

    if (pid == 0) // Child process
    {
        process_files(argv[1], pfd);
    }
    else if (pid > 0) // Parent process
    {
        process_dir(argv[1], pfd);
    }

    return 0;
}