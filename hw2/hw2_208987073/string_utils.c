#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>



char strip(char c)
{
    // Replaces special characters with '\0'
    if (c < 20) {
        return '\0';
    }
    return c;
}


bool string_starts_with(char* str, char* prefix)
{
    // Returns true iff str starts with the given prefix
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


void get_substring(char** str_ptr, char** sub_str_ptr, int ind)
{
    // Changes sub_str_ptr <- str_ptr[ind:]
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


void string_copy(char** dst_ptr, char** src_ptr) {
    // Copies a string into another one
    char* dst = (char*) calloc (strlen(*src_ptr)+1, sizeof(char));
    assert(dst);
    strcpy(dst, *src_ptr);
    *dst_ptr = dst;
}


int count_words(char* line) {
    // Counts the word amound in the line
    int count=0;
    char* token=NULL;
    char* line_copy=NULL;
    string_copy(&line_copy, &line);
    token = strtok(line_copy, ";");
    while(token != NULL) {
        if (token[0] == 10) return count;
        count++;
        token = strtok(NULL, ";");
    }  
    free(line_copy);
    line_copy = NULL;
    return count; 
}


char** split(char* line)
{
    // Splits the line into an array of commands
    int i=0, len = count_words(line);
    for (int ind=0; ind<len; ind++) {
        line[ind] = strip(line[ind]);
    }
    char** res = (char**) calloc (len, sizeof(char*));
    assert(res);
    char* token=NULL;
    char* line_copy=NULL;
    string_copy(&line_copy, &line);
    token = strtok(line_copy, ";");
    while (token != NULL){
        res[i] = (char*) calloc (strlen(token)+1, sizeof(char));
        assert(res[i]);
        strcpy(res[i], token);
        if (res[i][0] == ' '){
            for (int j=0; j<(int) strlen(token); j++){
                res[i][j] = res[i][j+1];
            }
        }
        i++;
        token = strtok(NULL, ";");
    }
    free(line_copy);
    line_copy = NULL;
    return res;
}


int get_replace_index(char** commands, int len){
    for (int i=0; i<len; i++){
        if(string_starts_with(commands[i], "repeat")){
            return i;
        }
    }
    return -1;
}


long long get_repeat_num(char* command){
    int ind=0;
    int len = strlen(command);
    for (int i=0; i<len; i++){
        if (command[i]==' '){
            ind = i;
            break;
        }
    }
    char* substring; 
    get_substring(&command, &substring, ind+1);
    long long res = strtoll(substring, NULL, 10);
    free(substring);
    substring = NULL;
    return res;
}


void replace_repeat(char*** commands_ptr, int* len_ptr){
    char** commands = *commands_ptr;
    int replace_index = get_replace_index(commands, *len_ptr);
    if (replace_index == -1){
        return;
    }
    int ind = -1;
    int repetitions = get_repeat_num(commands[replace_index]);
    int tail_len = *len_ptr - replace_index -1;
    int new_len = replace_index + repetitions*tail_len;
    char* new_command = NULL;
    char** new_commands = (char**) calloc (new_len, sizeof(char*));
    assert(new_commands);
    for (int i=0; i<new_len; i++){
        if (i<replace_index){
            new_command = commands[i];
        }
        else{
            ind = replace_index + 1 + ((i)%tail_len);
            new_command = commands[ind];
        }
        new_commands[i] = (char*) calloc (strlen(new_command)+1, sizeof(char));
        assert(new_commands[i]);
        strcpy(new_commands[i], new_command);
    }
    for (int i=0; i<*len_ptr; i++){
        free(commands[i]);
        commands[i] = NULL;
    }
    free(commands);
    commands = NULL;
    *commands_ptr = new_commands;
    *len_ptr = new_len;

}