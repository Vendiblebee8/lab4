#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid;

    child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (child_pid == 0) {
 
        printf("IN CHILD: pid=%d\n", getpid());

        execl("/bin/ls", "ls", "-l", "-a", NULL);

        perror("execl");
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
