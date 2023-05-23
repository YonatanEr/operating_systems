#include "system.utils.h"


bool is_bg_command(char* command) {
    //returns true iff command is a background one
    int i=strlen(command);
    while (1<=i) {
        i--;
        if (command[i] == '&') {
            return true;
        }
        if (command[i] > 32) {
            return false;
        }
    }
    return false;
}


bool is_exit_command(char* command) {
    //returns true iff command is an exit one
    if (string_starts_with(command, "exit")){
        return command[4] <= 32;
    } 
    return false;
}


bool is_cd_command(char* command) {
    //returns true iff command is a cd one
    if (string_starts_with(command, "cd")){
        return command[2] <= 32;
    } 
    return false;
}


bool is_jobs_command(char* command) {
    //returns true iff command is a jobs one
    if (string_starts_with(command, "jobs")){
        return command[4] <= 32;
    } 
    return false;
}


void execute_command(char* command, bool* terminate_ptr, pid_t bg_PIDs[], char* bg_commands[], int *bg_processes_counter_ptr) {
    // tests which command the user asked for and executes it
    if (is_exit_command(command)) {
        *terminate_ptr = true;
        return;
    }
    if (is_cd_command(command)) {
        execute_cd(command);
        return;
    }
    if (is_jobs_command(command)) {
        execute_jobs(bg_PIDs, bg_commands, bg_processes_counter_ptr);
        return;
    }
    execute_external_command(command);
}


void execute_cd(char* command) {
    // executes cd command
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


void execute_jobs(pid_t bg_PIDs[], char* bg_commands[], int *bg_processes_counter_ptr) {
    // executes jobs command
    fprintf_jobs(bg_PIDs, bg_commands, bg_processes_counter_ptr);
}


void execute_external_command(char* command) {
    // executes external command
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


void execute_bg_command(char* command, bool* terminate_ptr, pid_t bg_PIDs[], char* bg_commands[], int *bg_processes_counter_ptr) {
    // executes bg command
    int saved_errno = errno, cur_processes = *bg_processes_counter_ptr;
    pid_t child_pid = fork();
    if (child_pid < 0)
        fprintf_syscall_fail("fork", errno);
    else if (child_pid == 0) {
        execute_command(command, terminate_ptr, bg_PIDs, bg_commands, bg_processes_counter_ptr);
        exit(1);
    }
    else {
        fprintf_bg_process_started(child_pid);
        bg_PIDs[*bg_processes_counter_ptr] = child_pid;
        bg_commands[*bg_processes_counter_ptr] = (char*) calloc (strlen(command)+1, sizeof(char));
        assert(bg_commands[*bg_processes_counter_ptr]);
        strcpy(bg_commands[*bg_processes_counter_ptr], command);
        cur_processes++;
        *bg_processes_counter_ptr = cur_processes;
    }
    errno = saved_errno;
}