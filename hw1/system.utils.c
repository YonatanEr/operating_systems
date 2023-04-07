#include "system.utils.h"


bool is_background_command(char* command) {
    return (command[strlen(command)-1] == '&');
}

void execute_command(char* command, bool* terminate_ptr) {
    if (is_empty_line(command)) {
        return;
    }
    if (string_starts_with(command, "exit")) {
        execute_exit(command);
        *terminate_ptr = true;
        return;
    }
    if (string_starts_with(command, "cd")) {
        execute_cd(command);
        return;
    }
    if (string_starts_with(command, "jobs")) {
        execute_jobs(command);
        return;
    }
    execute_external_command(command);
}


void execute_exit(char* command) {
    int res = 0;
    if (res != 0){
        fprintf_invalid_command();
        fprintf_syscall_fail(command, errno);
    }
}


void execute_cd(char* command) {
    int res = chdir(command);
    if (res != 0){
        fprintf_invalid_command();
        fprintf_syscall_fail(command, errno);
    }
}


void execute_jobs(char* command) {
    int res = 0;
    if (res  != 0) { 
        fprintf_invalid_command();
        fprintf_syscall_fail(command, errno);
    }
}


void execute_external_command(char* command) {
    int i, words_amount = count_words(command);
    char* words[words_amount+1];
    char* command_copy;
    char* token;
    string_copy(&command_copy, &command);
    token = strtok(command_copy, " ");
    for (i=0; i<words_amount; i++) {
        words[i] = (char*) malloc ((strlen(token)+1) * sizeof(char));
        assert(words[i]);
        strcpy(words[i], token);
        token = strtok(NULL, " ");
    }
    free(command_copy);
    words[words_amount] = NULL;

    execute_subprocess(words);

    for (i=0; i<words_amount; i++) {
        free(words[i]);
    }
} 


void execute_subprocess(char* words[]) {
    int returnStatus;
    pid_t child_pid = fork();
    if (child_pid < 0) {        
        fprintf_syscall_fail("fork", errno); 
    }
    else if (child_pid == 0) {        
        execvp(words[0], words);
        exit(1);
    }
    else {
        waitpid(child_pid, &returnStatus, 0);
        if (returnStatus != 0)      
        {
            fprintf_invalid_command();
        }
    }
}
