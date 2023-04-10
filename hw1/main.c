#define MaxBackgroundProcesses 4

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "print.format.h"
#include "string.utils.h"
#include "system.utils.h"

void handle_exit(){
    return;
}


void clean_zombie_processes(){
    return;
}


bool validate_background(char* command, int* background_processes_counter_ptr) {
    if (!is_background_command(command)) {
        return true;
    }
    if (*background_processes_counter_ptr < MaxBackgroundProcesses){
        (*background_processes_counter_ptr) ++; 
        return true;
    }
    fprintf_background_overflow();
    return false;
}


int main() {
    bool terminate = false;
    int background_processes_counter=0;
    char* command;
    while (!terminate) {
        fprintf_shell();
        read_line(&command);
        if (validate_background(command, &background_processes_counter)) {
            execute_command(command, &terminate);
        }
        free(command);
        command = NULL;
        clean_zombie_processes();
    }
    handle_exit();
    return 0;
}

