#ifndef STRING_UTILS_H
#define STRING_UTILS_H


char strip(char c);

bool string_starts_with(char* str, char* prefix);

void get_substring(char** str_ptr, char** sub_str_ptr, int ind);

int count_words(char* line);

char** split(char* line);

void replace_repeat(char*** commands_ptr, int* len_ptr);

#endif
