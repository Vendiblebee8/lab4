#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define INPUT_FILE "./input/if2"

int main(int argc, char *argv[]) {
    int input_fd, output_fd;

    // Open the input file for reading
    input_fd = open(INPUT_FILE, O_RDONLY);
    if (input_fd == -1) {
        perror("Failed to open input file");
        exit(1);
    }

    // Create or truncate the output file for writing
    output_fd = open("result", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("Failed to open output file");
        exit(1);
    }

    pid_t child_pid;

    // Fork a child process
    child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (child_pid == 0) {
        // Child process

        // Re-associate STDIN_FILENO with the input file descriptor
        if (dup2(input_fd, STDIN_FILENO) == -1) {
            perror("Failed to redirect stdin");
            exit(1);
        }

        // Re-associate STDOUT_FILENO with the output file descriptor
        if (dup2(output_fd, STDOUT_FILENO) == -1) {
            perror("Failed to redirect stdout");
            exit(1);
        }

        // Close the original file descriptors
        close(input_fd);
        close(output_fd);

        // Execute the user-specified command with its options
        execvp(argv[1], &argv[1]);

        // If execvp fails, print an error message
        perror("execvp");
        exit(1);
    } else {
        // Parent process

        // Wait for the child to complete
        int status;
        wait(&status);

        if (WIFEXITED(status)) {
            // Child exited successfully
            printf("In PARENT: successfully waited child (pid=%d)\n", child_pid);
        } else {
            // Child did not exit normally
            printf("In PARENT: child process (pid=%d) did not exit normally\n", child_pid);
        }

        // Close the file descriptors
        close(input_fd);
        close(output_fd);
    }

    return 0;
}
