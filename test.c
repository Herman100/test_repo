#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

void execute_command(char *line, char **oldpwd) {
   // printf("Executing command: %s\n", line);
	char *args[MAX_ARGS];

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
    } else if (strcmp(args[0], "setenv") == 0) {
        if (args[1] == NULL || args[2] == NULL) {
            fprintf(stderr, "setenv: missing argument\n");
        } else {
            if (setenv(args[1], args[2], 1) == -1) {
                perror("setenv");
            }
        }
    } else if (strcmp(args[0], "unsetenv") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "unsetenv: missing argument\n");
        } else {
            if (unsetenv(args[1]) == -1) {
                perror("unsetenv");
            }
        }
    } else if (strcmp(args[0], "cd") == 0) {
        char *path;
        if (args[1] == NULL) {
            path = getenv("HOME");
            if (path == NULL) {
                fprintf(stderr, "cd: HOME not set\n");
                return;
            }
        } else if (strcmp(args[1], "-") == 0) {
            path = *oldpwd;
            if (path == NULL) {
                fprintf(stderr, "cd: OLDPWD not set\n");
                return;
            }
        } else {
            path = args[1];
        }

        char *cwd = getcwd(NULL, 0);
        if (chdir(path) == -1) {
            perror("cd");
        } else {
            setenv("PWD", path, 1);
            free(*oldpwd);
            *oldpwd = cwd;
        }
    } else {
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
}

int main() {
    char line[MAX_LINE];
    char *oldpwd = NULL;
    while (1) {
        printf("shell$ ");
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            break;
        }
        line[strcspn(line, "\n")] = '\0';

        // split the line into commands
	char *line_copy = strdup(line);
        char *command = line_copy;
        while ((command = strsep(&line_copy, ";")) != NULL) {
            //printf("Found command: %s\n", command);
            execute_command(command, &oldpwd);
        }
        free(line_copy);
    }

    free(oldpwd);
    return 0;
}

