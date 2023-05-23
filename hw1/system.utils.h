#ifndef SYSTEM_UTILS_H
#define SYSTEM_UTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
#include "print.format.h"
#include "string.utils.h"

bool is_bg_command(char* command);

void execute_command(char* command, bool* terminate_ptr, pid_t bg_PIDs[], char* bg_commands[], int *bg_processes_counter_ptr);

void execute_cd(char* command);

void execute_jobs(pid_t bg_PIDs[], char* commands[], int *bg_processes_counter_ptr);

void execute_external_command(char* command);

void execute_bg_command(char* command, bool* terminate_ptr, pid_t bg_PIDs[], char* bg_commands[], int *bg_processes_counter_ptr);

#endif

