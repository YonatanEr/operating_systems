#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include "dispatcher.h"
#include "string_utils.h"


int64_t millis()
{
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    return ((int64_t) now.tv_sec) * 1000 + ((int64_t) now.tv_nsec) / 1000000;
}


void statistics(Dispatcher* dispatcher, int64_t total_run_time)
{
    FILE* file = NULL;
    Thread* thread = NULL;
    int64_t sum_of_jobs_time = 0;
    int64_t min_job_time = -1;
    int64_t max_job_time = -1;
    long long total_jobs_amount = 0;
    for (int i=0; i<dispatcher->num_threads; i++){
        thread = dispatcher->threads[i];
        sum_of_jobs_time = sum_of_jobs_time + thread->total_run_time;
        if (min_job_time == -1){
            min_job_time = thread->min_time;
        }
        if (0 < thread->min_time && thread->min_time < min_job_time){
            min_job_time = thread->min_time;
        }
        if (max_job_time == -1){
            max_job_time = thread->max_time;
        }
        if (0 < thread->max_time && thread->max_time > max_job_time){
            max_job_time = thread->max_time;
        }
        total_jobs_amount = total_jobs_amount + thread->jobs_counter;
    }
    int64_t avg_job_time = 0;
    if (total_jobs_amount > 0){
        avg_job_time = sum_of_jobs_time/total_jobs_amount;
    }
    char stats_file[] = "stats.txt";
    file = fopen(stats_file, "w");
    assert(file);
    if (min_job_time == -1){
        min_job_time = 0;
    }
    if (max_job_time == -1){
        max_job_time = 0;
    }
    fprintf(file, "total running time: %lld\n", total_run_time);
    fprintf(file, "sum of jobs turnaround time: %lld\n", sum_of_jobs_time);
    fprintf(file, "min job turnaround time: %lld\n", min_job_time);
    fprintf(file, "average job turnaround time: %lld\n", avg_job_time);
    fprintf(file, "max job turnaround time: %lld\n", max_job_time);        
    fclose(file);
}


void busy_thread(Thread* thread){
    char* command = NULL;
    FILE* file = NULL;
    if (thread->log_enabled){
        char thread_log_file[] = "threadxxxx.txt";
        thread_log_file[6] = (thread->index/1000)%10 + '0';
        thread_log_file[7] = (thread->index/100)%10 + '0';
        thread_log_file[8] = (thread->index/10)%10 + '0';
        thread_log_file[9] = (thread->index/1)%10 + '0';
        file = fopen(thread_log_file, "a");
        assert(file);
    }
    int64_t start_time = millis() - thread->total_start_time;
    int64_t run_time = 0;
    for (int i=0; i<thread->commands_amount; i++){
        thread->jobs_counter++;
        command = thread->commands[i];
        long long start_time = thread->start_time;
        if (thread->log_enabled){
            fprintf(file, "TIME %lld: START job %s\n", start_time, command);
        }
        if (string_starts_with(command, "msleep ")){
            msleep(command);
        }
        if (string_starts_with(command, "increment ")){
            increment(thread, command);
        }
        if (string_starts_with(command, "decrement ")){
            decrement(thread, command);
        }
        int64_t finish_time = millis() - thread->total_start_time;
        if (thread->log_enabled){
                fprintf(file, "TIME %lld: END job %s\n", finish_time, command);
        }
        free(command);
        command = NULL;
        int64_t run_time = finish_time - start_time;
        thread->total_run_time = thread->total_run_time + run_time;
        if (thread->min_time == -1){
            thread->min_time = run_time;
        }
        if (run_time < thread->min_time){
            thread->min_time = run_time;
        }
        if (thread->max_time == -1){
            thread->max_time = run_time;
        }
        if (run_time > thread->max_time){
            thread->max_time = run_time;
        }
    }
    fclose(file);
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
        usleep(rand()%1000);
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
    int64_t total_start_time = millis();
    size_t buffer_len = 0;
    char* line = NULL;
    char** commands = NULL;
    int len = 0;
    Dispatcher* dispatcher = setup_dispatcher(argv, total_start_time);
    create_threads(dispatcher);
    FILE* file = fopen(dispatcher->filename, "r");
    while (true){
        if (getline(&line, &buffer_len, file) == -1) {
            fclose(file);
            free(line);
            line = NULL;
            break;
        }
        dispatcher->start_time = millis() - total_start_time;
        if (string_starts_with(line, "worker ") && line[7]!=';'){
            line[6] = ';';
        }
        len = count_words(line);       
        for (int c=0; c<(int) strlen(line); c++){
            if (line[c]=='\n'){
                line[c] = '\0';
            }
        }
        commands = split(line);
        if (dispatcher->log_enabled){
            FILE* file = NULL;
            file = fopen("dispatcher.txt", "a");
            assert(file);
            fprintf(file, "TIME %lld: read cmd line %s\n", millis()-total_start_time, line);
            fclose(file);
        }
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
    }
    for (int i=0; i<dispatcher->num_threads; i++){
        dispatcher_wait(dispatcher);
        pthread_kill(dispatcher->threads[i]->tid, 0);
    }
    statistics(dispatcher, millis()-total_start_time);
    free_dispatcher(dispatcher);
    return 0;
    
}
