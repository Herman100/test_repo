#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CMD_LEN 128

int main() {
    char cmd[MAX_CMD_LEN];
    while (1) {
        printf("shell$ ");
        if (fgets(cmd, MAX_CMD_LEN, stdin) == NULL) {
            printf("\n");
            break;
        }
        cmd[strcspn(cmd, "\n")] = '\0';
        if (strlen(cmd) == 0) continue;
        if (system(cmd) == -1) {
            printf("./shell: No such file or directory");
        }
    }
    return 0;
}
