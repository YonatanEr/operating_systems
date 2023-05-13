#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include "dispatcher.h"
#include "string_utils.h"


void busy_thread(Thread* thread){
    char* command = NULL;
    for (int i=0; i<thread->commands_amount; i++){
        command = thread->commands[i];
        if (string_starts_with(command, "msleep ")){
            msleep(command);
        }
        if (string_starts_with(command, "increment ")){
            increment(thread, command);
        }
        if (string_starts_with(command, "decrement ")){
            decrement(thread, command);
        }
        free(command);
        command = NULL;
    }
    free(thread->commands);
    thread->commands = NULL;
    thread->commands_amount=0;
    thread->busy = false;
}


void* run_thread(void* args)
{
    Thread* thread = args;
    char* command = NULL;
    while (true){
        if (thread->busy){
            busy_thread(thread);
        }
        usleep(1);
    }
}


void dispatcher_command(Dispatcher* dispatcher, char* command)
{
    if (string_starts_with(command, "dispatcher_msleep")){
        msleep(command);
    }
    if (string_starts_with(command, "dispatcher_wait")){
        dispatcher_wait(dispatcher);
    }
}


void thread_commands(Dispatcher* dispatcher, char** commands, int len)
{
    Thread* thread = NULL;
    int ind = -1;
    if (string_starts_with(commands[0], "worker")){
        ind = get_unbusy_thread(dispatcher);
        thread = dispatcher->threads[ind];
        thread->start_time = dispatcher->start_time;
        thread->commands_amount = len-1;
        thread->commands = (char**) calloc (len-1, sizeof(char*));
        assert(thread->commands);
        for (int i=1; i<len; i++){
            thread->commands[i-1] = (char*) calloc (strlen(commands)+1, sizeof(char));
            assert(thread->commands[i-1]);
            strcpy(thread->commands[i-1], commands[i]);
        }
        dispatcher->threads[ind]->busy = true;
    }
    else{
        for (int i=0; i<len; i++){
            ind = get_unbusy_thread(dispatcher);
            thread = dispatcher->threads[ind];
            thread->start_time = dispatcher->start_time;
            thread->commands_amount = 1;
            thread->commands = (char**) calloc (1, sizeof(char*));
            assert(thread->commands);
            thread->commands[0] = (char*) calloc (strlen(commands[i])+1, sizeof(char));
            assert(thread->commands[0]);
            strcpy(thread->commands[0], commands[i]);
            thread->busy = true;
        }
    }
}


void create_threads(Dispatcher* dispatcher)
{
    Thread* thread = NULL;
    for (int i=0; i<dispatcher->num_threads; i++){
        thread = dispatcher->threads[i];
        pthread_create(&thread->tid, NULL, run_thread, thread);
    }
}



void run_command(Dispatcher* dispatcher, char** commands, int len)
{   
    // Runs the input comands
    if (string_starts_with(commands[0], "dispatcher")) {
        dispatcher_command(dispatcher, commands[0]);
    }
    else {
        thread_commands(dispatcher, commands, len);
    }
}


int main(int argc, char *argv[])
{   
    // Runs the main flow
    assert(argc==5);
    time_t start_time = time(NULL);
    size_t buffer_len = 0;
    char* line = NULL;
    char** commands = NULL;
    int len = 0;
    Dispatcher* dispatcher = setup_dispatcher(argv);
    create_threads(dispatcher);
    FILE* file = fopen(dispatcher->filename, "r");
    while (true){
        if (getline(&line, &buffer_len, file) == -1) {
            fclose(file);
            free(line);
            line = NULL;
            break;
        }
        dispatcher->start_time = time(NULL);
        if (string_starts_with(line, "worker ") && line[7]!=';'){
            line[6] = ';';
        }
        len = count_words(line);       
        commands = split(line);
        free(line);
        line = NULL;
        replace_repeat(&commands, &len);
        run_command(dispatcher, commands, len);
        for (int i=0; i<len; i++){
            free(commands[i]);
            commands[i] = NULL;
        }
        free(commands);
        commands = NULL;
        dispatcher_wait(dispatcher);
    }
    for (int i=0; i<dispatcher->num_threads; i++){
        pthread_kill(dispatcher->threads[i]->tid, 0);
    }
    free_dispatcher(dispatcher);
    return 0;
}

