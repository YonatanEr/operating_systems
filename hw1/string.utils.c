#include "string.utils.h"


void read_line(char** line_ptr) {
    size_t size = MaxLineLength;
    char* line = (char*) malloc ((size+1)*sizeof(char));
    assert(line);
    line[size] = '\0';
    getline(&line, &size, stdin);
    *line_ptr = line;
}



bool string_starts_with(char* str, char* prefix) {
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
    char* dst = (char*) calloc (strlen(*src_ptr)+1, sizeof(char));
    assert(dst);
    strcpy(dst, *src_ptr);
    *dst_ptr = dst;
}


int count_words(char* line) {
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
    return count; 
}


bool is_empty_line(char* line) {
    return count_words(line) == 0;
}


char rstrip(char c) {
    if (c < 20) {
        return '\0';
    }
    return c;
}
