#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define INPUT_FILE "./input/if2"

int main(int argc, char *argv[]) {
    int input_fd, output_fd;

    input_fd = open(INPUT_FILE, O_RDONLY);
    if (input_fd == -1) {
        perror("Failed to open input file");
        exit(1);
    }

    output_fd = open("result", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("Failed to open output file");
        exit(1);
    }

    pid_t child_pid;

    child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (child_pid == 0) {

        if (dup2(input_fd, STDIN_FILENO) == -1) {
            perror("Failed to redirect stdin");
            exit(1);
        }

        if (dup2(output_fd, STDOUT_FILENO) == -1) {
            perror("Failed to redirect stdout");
            exit(1);
        }

        close(input_fd);
        close(output_fd);

        execvp(argv[1], &argv[1]);

        perror("execvp");
        exit(1);
    } else {

        int status;
        wait(&status);

        if (WIFEXITED(status)) {
            printf("In PARENT: successfully waited child (pid=%d)\n", child_pid);
        } else {
            printf("In PARENT: child process (pid=%d) did not exit normally\n", child_pid);
        }

        close(input_fd);
        close(output_fd);
    }

    return 0;
}
