#ifndef DISPATCHER_H
#define DISPATCHER_H

#define MAX_NUM_THREADS 4096
#define MAX_NUM_COUNTERS 100

#include <stdbool.h>
#include <pthread.h>
#include <inttypes.h>


typedef struct MyThread {
    // Threads
    long long jobs_counter;
    int64_t start_time;
    int64_t total_start_time;
    int64_t total_run_time;
    int64_t min_time;
    int64_t max_time;
    pthread_t tid;
    int index;  
    bool busy;
    int commands_amount; 
    bool log_enabled;
    char** commands;
} Thread;



typedef struct MyDispatcher {
    // Dispatcher
    int64_t start_time;
    int64_t total_start_time;
    char* filename;
    int num_threads;
    int num_counters;
    bool log_enabled;
    Thread** threads;
} Dispatcher;


int get_unbusy_thread(Dispatcher* dispatcher);

void msleep(char* command);

void thread_change_counter(int i, int change);

void dispatcher_wait(Dispatcher* dispatcher);

Dispatcher* setup_dispatcher(char *argv[], time_t total_start_time);

void free_dispatcher(Dispatcher* dispatcher);

void print_thread(Thread* thread);

void print_threads(Thread** threads, int num_threads);

void print_dispatcher(Dispatcher* dispatcher);

#endif
