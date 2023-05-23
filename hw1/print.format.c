#include "print.format.h"


void fprintf_shell(){
    fprintf(stdout, "hw1shell$ ");
}


void fprintf_invalid_command(){
    fprintf(stdout, "hw1shell: invalid command\n");
}


void fprintf_bg_overflow(){
    fprintf(stdout, "hw1shell: too many bg commands running\n");
}


void fprintf_bg_process_started(int pid) {
    fprintf(stdout, "hw1shell: pid %d started\n", pid);
}


void fprintf_bg_process_finished(int pid) {
    fprintf(stdout, "hw1shell: pid %d finished\n", pid);
}


void fprintf_syscall_fail(char* syscall_name, int error_number) {
    fprintf(stdout, "hw1shell: %s failed, errno is %d\n", syscall_name, error_number);
}


void fprint_words(char** words) {
    int i=0;
    while (words[i] != NULL) {
        fprintf(stdout, "word[%d] = %s\n", i, words[i]);
        i++;
    }
}


void fprintf_command(char* command) {
    fprintf(stdout, "%s\n", command);
}


void fprintf_process(pid_t pid, char* command) {
    fprintf(stdout, "%d\t%s\n", pid, command);
}


void fprintf_jobs(pid_t bg_PIDs[], char* bg_commands[], int *bg_processes_counter_ptr) {
    for (int i = 0; i < *bg_processes_counter_ptr; i++) {
        fprintf_process(bg_PIDs[i], bg_commands[i]);
    }
}
