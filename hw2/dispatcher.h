#ifndef DISPATCHER_H
#define DISPATCHER_H

#define MAX_NUM_THREADS 4096
#define MAX_NUM_COUNTERS 100

#include <stdbool.h>
#include <pthread.h>


typedef struct MyThread {
    // Threads
    time_t start_time;
    pthread_t tid;
    int index;  
    bool busy;
    bool* busy_counter;
    int commands_amount; 
    char** commands;
} Thread;



typedef struct MyDispatcher {
    // Dispatcher
    time_t start_time;
    char* filename;
    int num_threads;
    int num_counters;
    bool log_enabled;
    Thread** threads;
} Dispatcher;


int get_unbusy_thread(Dispatcher* dispatcher);

void msleep(char* command);

void increment(Thread* thread, char* command);

void decrement(Thread* thread, char* command);

void dispatcher_wait(Dispatcher* dispatcher);

Dispatcher* setup_dispatcher(char *argv[]);

void free_dispatcher(Dispatcher* dispatcher);

void print_thread(Thread* thread);

void print_threads(Thread** threads, int num_threads);

void print_dispatcher(Dispatcher* dispatcher);

#endif
