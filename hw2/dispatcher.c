#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "dispatcher.h"
#include "string_utils.h"


void thread_change_counter(int i, int change)
{
    // Change the counter at index i
    long long counter = 0;
    FILE* file = NULL;
    size_t len = 0;
    char* line = NULL;
    char counter_file[] = "countxx.txt";
    counter_file[5] = (i/10)%10 + '0';
    counter_file[6] = (i/1)%10 + '0';
    file = fopen(counter_file, "r");
    assert(file);
    getline(&line, &len, file);
    fclose(file);
    counter = strtoll(line, NULL, 10);
    file = fopen(counter_file, "w");
    assert(file);
    fprintf(file, "%lld", counter+change);
    fclose(file);
}


////////////////////////////////////////////////////////////////////////


print_stats(Dispatcher* dispatcher)
{

}

////////////////////////////////////////////////////////////////////////


int get_unbusy_thread(Dispatcher* dispatcher)
{
    // Find a none busy thread
    int i = 0;
    while (true)
    {
        if (!dispatcher->threads[i]->busy){
            return i;
        }
        i = (i+1)%(dispatcher->num_threads);
    }   
}


////////////////////////////////////////////////////////////////////////


void msleep(char* command)
{
    // Sleeps for the specified in milliseconds
    char* sub_str = NULL;
    get_substring(&command, &sub_str, 7);
    int time = strtol(sub_str, NULL, 10);
    free(sub_str);
    sub_str = NULL;
    usleep(time*1000);   
}


void dispatcher_wait(Dispatcher* dispatcher)
{
    // Find a none busy thread
    for (int i=0; i<dispatcher->num_threads; i++){
        while (dispatcher->threads[i]->busy){
            usleep(1);
        }
    }
}


void increment(Thread* thread, char* command)
{
    // Increment the specified file counter
    char* sub_str = NULL;
    get_substring(&command, &sub_str, 10);
    int i = strtol(sub_str, NULL, 10);
    free(sub_str);
    sub_str = NULL;
    while (thread->busy_counter[i]){
        usleep(rand()%1000);
    }
    thread->busy_counter[i] = true;
    thread_change_counter(i, 1);
    thread->busy_counter[i] = false;
}


void decrement(Thread* thread, char* command)
{
    // Decrement the specified file counter
    char* sub_str = NULL;
    get_substring(&command, &sub_str, 10);
    int i = strtol(sub_str, NULL, 10);
    free(sub_str);
    sub_str = NULL;
    while (thread->busy_counter[i]){
        usleep(rand()%1000);
    }
    thread->busy_counter[i] = true;
    thread_change_counter(i, -1);
    thread->busy_counter[i] = false;
}


////////////////////////////////////////////////////////////////////////


void create_counter_files(Dispatcher* dispatcher)
{   
    // Creates the count files
    FILE* file = NULL;
    char counter_file[] = "countxx.txt";
    for (int i=0; i<dispatcher->num_counters; i++){
        counter_file[5] = (i/10)%10 + '0';
        counter_file[6] = (i/1)%10 + '0';
        file = fopen(counter_file, "w");
        assert(file);
        fprintf(file, "%lld", 0);        
        fclose(file);
    }
}


void create_thread_log_files(Dispatcher* dispatcher)
{   
    // Creates the threads log files 
    FILE* file = NULL;
    char thread_log_file[] = "threadxxxx.txt";
    for (int i=0; i<dispatcher->num_threads; i++){
        thread_log_file[6] = (i/1000)%10 + '0';
        thread_log_file[7] = (i/100)%10 + '0';
        thread_log_file[8] = (i/10)%10 + '0';
        thread_log_file[9] = (i/1)%10 + '0';
        file = fopen(thread_log_file, "w");
        assert(file);
        fclose(file);
    }
}


void create_dispatcher_log_files(Dispatcher* dispatcher)
{   
    // Creates the dispacher log files 
    FILE* file = fopen("dispatcher.txt", "w");
    assert(file);
    fclose(file);
}


////////////////////////////////////////////////////////////////////////


Thread* init_thread(int i)
{
    Thread* thread = (Thread*) calloc (1, sizeof(Thread));
    assert(thread);
    thread->tid = -1;
    thread->index = i;
    thread->busy = false;
    thread->commands_amount = -1;
    thread->commands = NULL;
    return thread;
}


Thread** init_threads(int num_threads, int num_counter)
{
    Thread** threads = (Thread**) calloc (num_threads, sizeof(Thread*));
    assert(threads);
    bool* busy_counter = (bool*) calloc (num_counter, sizeof(bool));
    assert(busy_counter);
    for (int i=0; i<num_counter; i++){
        busy_counter[i] = false;
    }
    for (int i=0; i<num_threads; i++){
        threads[i] = init_thread(i);
        threads[i]->busy_counter = busy_counter;
    }
    return threads;
}


Dispatcher* init_dispatcher(char *argv[])
{
    // Init a dispatcher
    Dispatcher* dispatcher = (Dispatcher*) calloc (1, sizeof(Dispatcher));
    assert(dispatcher);
    dispatcher->start_time = -1;
    dispatcher->filename = (char*) calloc (strlen(argv[1])+1, sizeof(char));
    assert(dispatcher->filename);
    strcpy(dispatcher->filename, argv[1]);
    dispatcher->num_threads = strtol(argv[2], NULL, 10);
    dispatcher->num_counters = strtol(argv[3], NULL, 10);
    dispatcher->log_enabled = strtol(argv[4], NULL, 10);
    dispatcher->threads = init_threads(dispatcher->num_threads, dispatcher->num_counters);
    return dispatcher;
}


Dispatcher* setup_dispatcher(char *argv[])
{
    // Setup the dispatcher
    Dispatcher* dispatcher = init_dispatcher(argv);
    create_counter_files(dispatcher);
    if (dispatcher->log_enabled){
        create_thread_log_files(dispatcher);
        create_dispatcher_log_files(dispatcher);
    }
    return dispatcher;
}


////////////////////////////////////////////////////////////////////////


void free_thread(Thread* thread)
{
    // Free a thread
    free(thread->commands);
    thread->commands = NULL;
}


void free_threads(Thread** threads, int num_threads)
{
    // Free the threads
    for (int i=0; i<num_threads; i++){
        free_thread(threads[i]);
    }
    if (num_threads>0){
        free(threads[0]->busy_counter);
        threads[0]->busy_counter = NULL;
    }
    threads[0]->busy_counter = NULL;
    free(threads);
    threads = NULL;
}


void free_dispatcher(Dispatcher* dispatcher)
{   
    // Free the dispatcher
    free(dispatcher->filename);
    dispatcher->filename = NULL;
    Thread** threads = dispatcher->threads;
    free_threads(dispatcher->threads, dispatcher->num_threads);
    free(dispatcher);
    dispatcher = NULL;
}


////////////////////////////////////////////////////////////////////////


void print_thread(Thread* thread)
{
    printf("\tstart_time = %lld\n", thread->start_time);
    printf("\ttid = %lld\n", thread->tid);
    printf("\tindex = %d\n", thread->index);
    printf("\tbusy = %d\n", thread->busy);
    printf("\tcommands_amount = %d\n", thread->commands_amount);
    for (int i=0; i<thread->commands_amount; i++){
        printf("\t\tcommand[%d] = %s\n", i, thread->commands[i]);
    }
    printf("\n");
}


void print_threads(Thread** threads, int num_threads)
{
    for (int i=0; i<num_threads; i++){
        print_thread(threads[i]);
        printf("\n");
    }
}


void print_dispatcher(Dispatcher* dispatcher)
{
    printf("start_time = %lld\n", dispatcher->start_time);
    printf("filename = %s\n", dispatcher->filename);   
    printf("num_threads = %d\n", dispatcher->num_threads);   
    printf("num_counters = %d\n", dispatcher->num_counters);   
    printf("log_enabled = %d\n", dispatcher->log_enabled);
    print_threads(dispatcher->threads, dispatcher->num_threads);
}

