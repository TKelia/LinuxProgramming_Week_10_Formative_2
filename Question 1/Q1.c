#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command>\n", argv[0]);
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child process: run the command
        printf("Child process running: %s\n\n", argv[1]);
        execvp(argv[1], &argv[1]);
        perror("exec failed");
        exit(1);
    } else {
        // Parent process
        wait(NULL);

        char message[256];
        printf("\nEnter a message to write to tempfile.txt: ");
        fgets(message, sizeof(message), stdin);

        // Remove trailing newline if present
        size_t len = strlen(message);
        if (len > 0 && message[len - 1] == '\n') {
            message[len - 1] = '\0';
        }

        FILE *file = fopen("tempfile.txt", "w+");
        if (!file) {
            perror("File error");
            return 1;
        }

        fprintf(file, "%s", message);
        rewind(file);

        char buffer[256];
        fgets(buffer, sizeof(buffer), file);
        printf("Read from tempfile: %s\n", buffer);

        fclose(file);
    }

    return 0;
}
