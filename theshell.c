#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 1024

int main() {
    char line[MAX_LINE];
    while (1) {
        printf("shell$ ");
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            break;
        }
        line[strcspn(line, "\n")] = '\0';

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // child process
            execl(line, line, (char *)NULL);
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
