#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

int main() {
    char line[MAX_LINE];
    char *args[MAX_ARGS];
    while (1) {
        printf("shell$ ");
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            break;
        }
        line[strcspn(line, "\n")] = '\0';

        // split the line into arguments
        int i = 0;
        args[i] = strtok(line, " ");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }

          // check for built-in commands
        if (strcmp(args[0], "exit") == 0) {
            int status = 0;
            if (args[1] != NULL) {
                status = atoi(args[1]);
            }
            exit(status);
        }

        pid_t pid = fork();
if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // child process
            execvp(args[0], args);
            perror("shell");
            exit(1);
        } else {
            // parent process
            int status;
            wait(&status);
        }
    }
    return 0;
}

