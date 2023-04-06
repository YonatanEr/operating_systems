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
    char command[MaxLineLength+1];
    int wstatus;
    pid_t pid;
    while (!feof(stdin) && !terminate) {
        fprintf(stdout, "hw1shell$ ");
        fgets(command, MaxLineLength+1, stdin);
        if (is_background_cmd(command)) {
            pid = fork();
            waitpid(pid, &wstatus, 0);
        }
        else {
            execute_command(command, &terminate);
        }
    }
    printf("process %d exiting\n", getpid());   
    return 0;
}
