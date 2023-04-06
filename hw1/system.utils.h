#ifndef SYSTEM_UTILS_H
#define SYSTEM_UTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
#include "string.utils.h"


void execute_command(char* line, bool* continue_reading_ptr);

void execute_exit(char* command);

void execute_cd(char* command);

void execute_jobs(char* command);

void execute_external_command(char* command);


#endif

