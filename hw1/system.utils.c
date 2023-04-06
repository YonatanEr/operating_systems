#include "system.utils.h"


void execute_command1(char* command, bool* terminate_ptr) {
    if (string_starts_with(command, "exit")) {
        execute_exit(command);
        *terminate_ptr = true;
    }
    else if (string_starts_with(command, "cd")) {
        execute_cd(command);
    }
    else if (string_starts_with(command, "jobs")) {
        execute_jobs(command);
    }
    else {
        execute_external_command(command);
    }
}


void execute_command(char* command, bool* terminate_ptr) {
    if (!*terminate_ptr) {
        execute_external_command(command);
    }
}


void execute_exit(char* command) {
    int res = 0;
    if (res != 0){
        fprintf(stdout, "hw1shell: invalid command\n");
        fprintf(stdout, "hw1shell$ %s failed, errno is %d\n", command, errno);
    }
}


void execute_cd(char* command) {
    int res = chdir(command);
    if (res != 0){
        fprintf(stdout, "hw1shell: invalid command\n");
        fprintf(stdout, "hw1shell$ %s failed, errno is %d\n", command, errno);
    }
}


void execute_jobs(char* command) {
    int res = 0;
    if (res  != 0) { 
        fprintf(stdout, "hw1shell$ %s failed, errno is %d\n", command, errno);
    }
}


void print_words(char** words) {
    int i=0;
    printf("\n\n\n");
    while (words[i] != NULL) {
        printf("%s", words[i]);
        fprintf(stdout, "word[%d] = %s\n", i, words[i]);
        i++;
    }
}


void execute_external_command(char* command) {
    int i, words_amount = count_words(command);
    char* words[words_amount+1];
    char* command_copy;
    char* token;
    string_copy(&command_copy, &command);
    token = strtok(command_copy, " ");
    for (i=0; i<words_amount; i++) {
        words[i] = (char*) malloc ((strlen(token)+1) * sizeof(char));
        assert(words[i]);
        strcpy(words[i], token);
        words[i][strlen(words[i])-1] = rstrip(words[i][strlen(words[i])-1]);
        token = strtok(NULL, " ");
    }
    free(command_copy);
    words[words_amount] = NULL;
    
    execvp(words[0], words);

    for (i=0; i<words_amount; i++) {
        free(words[i]);
    }
    perror("");
} 


