#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [option1] [option2] ...\n", argv[0]);
        exit(1);
    }

    pid_t child_pid;

    child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (child_pid == 0) {

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
    }

    return 0;
}
