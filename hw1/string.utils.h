#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#define MaxLineLength 1024   
#define MaxWords 64   

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include "print.format.h"
#include <string.h>


void read_line(char** line_ptr);

bool string_starts_with(char* str, char* prefix);

void string_copy(char** dst_ptr, char** src_ptr);
    
int count_words(char* line);

bool is_empty_line(char* line);

char strip(char c);

void get_substring(char** str_ptr, char** sub_str_ptr, int ind);


#endif

