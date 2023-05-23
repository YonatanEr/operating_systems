#define MaxbgProcesses 4

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "print.format.h"
#include "string.utils.h"
#include "system.utils.h"


void init(pid_t bg_PIDs[], int bg_status[], char* bg_commands[]) {
    for (int i = 0; i < MaxbgProcesses; i++) {
        bg_PIDs[i] = -1;
        bg_status[i] = -1;
        bg_commands[i] = NULL;
    }
}


void handle_exit(pid_t bg_PIDs[], int bg_status[], int* bg_processes_counter_ptr, char* bg_commands[]) {
    // executes exit command, terminates all the current running processes
    for (int i = 0; i < *bg_processes_counter_ptr; i++) {
        waitpid(bg_PIDs[i], &bg_status[i], 0);
        if (bg_status[i] != 0) {
                fprintf_syscall_fail("exec", errno); 
                fprintf_invalid_command();
            }
        free(bg_commands[i]);
        bg_commands[i] = NULL;
    }
}


void clean_zombie_processes(pid_t bg_PIDs[], int bg_status[], int *bg_processes_counter_ptr, char* bg_commands[]) {
    // cleans all the remaining processes which are at zombie mode
    int i=0, cur_processes = *bg_processes_counter_ptr, rc_pid;
    int count=0;
    pid_t new_bg_PIDs[MaxbgProcesses];
    int new_bg_status[MaxbgProcesses];
    char* new_bg_commands[MaxbgProcesses];
    init(new_bg_PIDs, new_bg_status, new_bg_commands);
    for (i=0; i<MaxbgProcesses; i++) {
        if (bg_commands[i] != NULL) {
            rc_pid = waitpid(bg_PIDs[i], &bg_status[i], WNOHANG);
            bg_status[i] = -1;
            if (rc_pid == -1) {
                fprintf_bg_process_finished(bg_PIDs[i]);
                cur_processes--;
            }
            else {
                new_bg_PIDs[count] = bg_PIDs[i];
                new_bg_status[count] = bg_status[i];
                new_bg_commands[count] = (char*) calloc (strlen(bg_commands[i]+1), sizeof(char));
                assert(new_bg_commands[count]);
                strcpy(new_bg_commands[count], bg_commands[i]);
                count++;
            }
        }
    }

    for (i=0; i<MaxbgProcesses; i++) {
        bg_PIDs[i] = new_bg_PIDs[i];
        bg_status[i] = new_bg_status[i];
        if (bg_commands[i] != NULL) {
            free(bg_commands[i]);
            bg_commands[i] = NULL;
        }
        if (new_bg_commands[i] != NULL) {
            bg_commands[i] = (char*) calloc (strlen(new_bg_commands[i]+1), sizeof(char));
            assert(bg_commands[i]);
            strcpy(bg_commands[i], new_bg_commands[i]);
            free(new_bg_commands[i]);
            new_bg_commands[i] = NULL;
        }
    }
    *bg_processes_counter_ptr = cur_processes;
    return;
}


bool validate_bg(char* command, int* bg_processes_counter_ptr) {
    //validates that the command can be ran
    if (!is_bg_command(command)) {
        return true;
    }
    if (*bg_processes_counter_ptr < MaxbgProcesses){
        return true;
    }
    fprintf_bg_overflow();
    return false;
}


int main() {
    pid_t bg_PIDs[MaxbgProcesses];
    int bg_status[MaxbgProcesses];
    char* bg_commands[MaxbgProcesses];
    int bg_processes_counter=0;
    char* command;
    bool terminate = false;

    init(bg_PIDs, bg_status, bg_commands);

    while (!terminate) {
        fprintf_shell();
        read_line(&command);
        if (validate_bg(command, &bg_processes_counter)) {
            if (is_bg_command(command)) {
                execute_bg_command(command, &terminate, bg_PIDs, bg_commands, &bg_processes_counter);
            }
            else {
                execute_command(command, &terminate, bg_PIDs, bg_commands, &bg_processes_counter);
            }
        }
        free(command);
        command = NULL;
        clean_zombie_processes(bg_PIDs, bg_status, &bg_processes_counter, bg_commands);
    }
    handle_exit(bg_PIDs, bg_status, &bg_processes_counter, bg_commands);
    return 0;
}

