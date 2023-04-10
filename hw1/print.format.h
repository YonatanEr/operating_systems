#ifndef PRINT_FORMAT_H
#define PRINT_FORMAT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>


void fprintf_shell();

void fprintf_invalid_command();

void fprintf_background_overflow();

void fprintf_background_process_started(pid_t pid);

void fprintf_background_process_finished(pid_t pid);

void fprintf_syscall_fail(char* syscall_name, int error_number);

void fprint_words(char** words);

void fprint_command(char* command);

#endif