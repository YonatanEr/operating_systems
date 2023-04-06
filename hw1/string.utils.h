#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#define MaxLineLength 1024   
#define MaxWords 64   

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


void parse_line(char* line, char*** words_ptr);

bool string_starts_with(char* str, char* prefix);


#endif

