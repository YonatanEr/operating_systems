#include "string.utils.h"


void parse_line(char* line, char*** words_ptr) {
    int i=0;
    char* token;
    char** words =(char**) calloc (MaxWords+1, sizeof(char*));
    assert(words);
    token = strtok(line, " ");
    while(token != NULL) {
        words[i] = (char*) calloc (strlen(token)+1, sizeof(char));
        strcpy(words[i], token);
        i++;
        token = strtok(NULL, " ");
    }
    *words_ptr = words;
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


