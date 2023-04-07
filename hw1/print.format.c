#include "print.format.h"


void fprintf_shell(){
    fprintf(stdout, "hw1shell$ ");
}


void fprintf_invalid_command(){
    fprintf(stdout, "hw1shell: invalid command\n");
}


void fprintf_background_overflow(){
    fprintf(stdout, "hw1shell: too many background commands running\n");
}


void fprintf_background_process_started(pid_t pid) {
    fprintf(stdout, "hw1shell: pid %d started\n", pid);
}


void fprintf_background_process_finished(pid_t pid) {
    fprintf(stdout, "hw1shell: pid %d finished\n", pid);
}


void fprintf_syscall_fail(char* syscall_name, int error_number) {
    fprintf(stdout, "hhw1shell: %s failed, errno is %d\n", syscall_name, error_number);
}


void fprint_words(char** words) {
    int i=0;
    fprintf(stdout, "\n\n\n");
    while (words[i] != NULL) {
        fprintf(stdout, "word[%d] = %s\n", i, words[i]);
        i++;
    }
}


void fprint_command(char* command) {
    fprintf(stdout, "%s", command);
}
