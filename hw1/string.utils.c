#include "string.utils.h"


void read_line(char** line_ptr) {
    // reads a line with max buffer of MaxLineLength
    int i;
    size_t size = MaxLineLength;
    char* line = (char*) calloc ((MaxLineLength+1), sizeof(char));
    assert(line);
    line[MaxLineLength] = '\0';
    getline(&line, &size, stdin);
    for (i=0; i<MaxLineLength; i++) {
        line[i] = strip(line[i]);
    }
    i = MaxLineLength;
    while(0<=i && line[i]<=32) {
        line[i] = '\0';
        i--;
    }
    *line_ptr = line;
}


bool string_starts_with(char* str, char* prefix) {
    // returns true iff str starts with the given prefix
    int len1 = strlen(str), len2 = strlen(prefix);
    if (len1 < len2){
        return false;
    }
    for (int i=0; i<len2; i++) {
        if (str[i] != prefix[i]) {
            return false;
        }
    }
    return true;
}


void string_copy(char** dst_ptr, char** src_ptr) {
    // copies a string into another one
    char* dst = (char*) calloc (strlen(*src_ptr)+1, sizeof(char));
    assert(dst);
    strcpy(dst, *src_ptr);
    *dst_ptr = dst;
}


int count_words(char* line) {
    // counts the word amound in the line
    int count=0;
    char* token;
    char* line_copy;
    string_copy(&line_copy, &line);
    token = strtok(line_copy, " ");
    while(token != NULL) {
        if (token[0] == 10) return count;
        count++;
        token = strtok(NULL, " ");
    }  
    free(line_copy);
    line_copy = NULL;
    return count; 
}


bool is_empty_line(char* line) {
    // returns true iff line is empty
    return count_words(line) == 0;
}


char strip(char c) {
    // replaces special characters with '\0'
    if (c < 20) {
        return '\0';
    }
    return c;
}


void get_substring(char** str_ptr, char** sub_str_ptr, int ind) {
    // changes sub_str_ptr <- str_ptr[ind:]
    char* str = *str_ptr;
    int i, str_len = strlen(str);
    char* res = (char*) calloc (str_len-ind+1, sizeof(char));
    assert(res);
    for (i=ind; i<str_len; i++) {
        res[i-ind] = str[i];
    }
    res[str_len-ind] = '\0';
    *sub_str_ptr = res;
}
