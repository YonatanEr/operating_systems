#ifndef PRINT_FORMAT_H
#define PRINT_FORMAT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>


void fprintf_shell();

void fprintf_invalid_command();

void fprintf_bg_overflow();

void fprintf_bg_process_started(int pid);

void fprintf_bg_process_finished(int pid);

void fprintf_syscall_fail(char* syscall_name, int error_number);

void fprint_words(char** words);

void fprintf_command(char* command);

void fprintf_process(pid_t pid, char* command);

void fprintf_jobs(pid_t bg_PIDs[], char* bg_commands[], int *bg_processes_counter_ptr);

#endif