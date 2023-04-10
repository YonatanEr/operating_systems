#include "system.utils.h"


bool is_background_command(char* command) {
    int i=strlen(command);
    while (0<=i && command[i]<=32) {
        if (command[i] == '&') {
            return true;
        }
        i--;
    }
    return false;
}


bool is_exit_command(char* command) {
    if (string_starts_with(command, "exit")){
        return command[4] <= 32;
    } 
    return false;
}


bool is_cd_command(char* command) {
    if (string_starts_with(command, "cd")){
        return command[2] <= 32;
    } 
    return false;
}


bool is_jobs_command(char* command) {
    if (string_starts_with(command, "jobs")){
        return command[2] <= 4;
    } 
    return false;
}


void execute_command(char* command, bool* terminate_ptr) {
    if (is_exit_command(command)) {
        execute_exit(command);
        *terminate_ptr = true;
        return;
    }
    if (is_cd_command(command)) {
        execute_cd(command);
        return;
    }
    if (is_jobs_command(command)) {
        execute_jobs(command);
        return;
    }
    execute_external_command(command);
}


void execute_exit(char* command) {
    int res = 0;
    if (res != 0){
        fprintf_invalid_command();
        fprintf_syscall_fail("exit", errno);
        fprint_command(command); // To be deleted
    }
}


void execute_cd(char* command) {
    if (count_words(command) != 2) { 
        fprintf_invalid_command();
        return;
    }
    char* directory;
    get_substring(&command, &directory, 3);
    int res = chdir(directory);
    free(directory);
    directory = NULL;
    if (res != 0){
        fprintf_invalid_command();
    }
}


void execute_jobs(char* command) {
    int res = 0;
    if (res  != 0) { 
        fprintf_invalid_command();
        fprintf_syscall_fail("jobs", errno);
        fprint_command(command); // To be deleted
    }
}


void execute_external_command(char* command) {
    int returnStatus, saved_errno = errno;
    pid_t child_pid = fork();
    char* arg_lst[] = {"sh", "-c", command, NULL};        
    
    if (child_pid < 0) {        
        fprintf_syscall_fail("fork", errno); 
    }

    else if (child_pid == 0) {      
        execvp("/bin/sh", arg_lst);   
        exit(1);
    }

    else {
        waitpid(child_pid, &returnStatus, 0);
        if (returnStatus != 0) {
            fprintf_syscall_fail("exec", errno); 
            fprintf_invalid_command();
        }
    }
    errno = saved_errno;
}
