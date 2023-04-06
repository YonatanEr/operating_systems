#include "system.utils.h"


void execute_command(char* command, bool* terminate_ptr) {
    if (command[0] == '\n') {
        return;
    }
    if (string_starts_with(command, "exit")) {
        execute_exit(command);
        *terminate_ptr = true;
    }
    else if (string_starts_with(command, "cd")) {
        execute_cd(command);
    }
    else if (string_starts_with(command, "jobs")) {
        execute_jobs(command);
    }
    else {
        execute_external_command(command);
    }
}


void execute_exit(char* command) {
    int res = 0;
    if (res != 0){
        fprintf(stdout, "hw1shell: invalid command\n");
        fprintf(stdout, "hw1shell$ %s failed, errno is %d\n", command, errno);
    }
}


void execute_cd(char* command) {
    int res = chdir(command);
    if (res != 0){
        fprintf(stdout, "hw1shell: invalid command\n");
        fprintf(stdout, "hw1shell$ %s failed, errno is %d\n", command, errno);
    }
}


void execute_jobs(char* command) {
    int res = 0;
    if (res  != 0) { 
        fprintf(stdout, "hw1shell$ %s failed, errno is %d\n", command, errno);
    }
}


void execute_external_command(char* command) {
    char** words;
    parse_line(command, &words);
    printf("execute_external_command\n");
    printf("COMMAND = %s\n", command);
    for (int i=0; i<MaxWords; i++) {
        if (words[i] == NULL) break;
        printf("%s\n", words[i]);
    }
    execv(words[0], words);
    free(words);
    perror("Return from execvp() not expected");
} 

