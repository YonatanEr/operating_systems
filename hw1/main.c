#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "string.utils.h"
#include "system.utils.h"


bool is_background_cmd(char* str) {
    return (str[strlen(str)-1] == '&');
}


int main() {
    bool terminate = false;
    int wstatus;
    pid_t pid;
    char* command;
    while (!feof(stdin) && !terminate) {
        fprintf(stdout, "hw1shell$ ");
        read_line(&command);
        if (is_empty_line(command)) {
            free(command);
            continue;
        }

        else if (is_background_cmd(command)) {
            pid = fork();
            execute_command(command, &terminate);
            waitpid(pid, &wstatus, 0);
        }

        else {
            execute_command(command, &terminate);
        }

        fprintf(stdout, "\n");
        free(command);
    }
    return 0;
}

