#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


// prints the string which fits the int state
int 
print_state(int state)
{
    if (state == 1){
        printf(1, "embryo");
    }
    if (state == 2){
        printf(1, "sleeping");
    }
    if (state == 3){
        printf(1, "runnable");
    }
    if (state == 4){
        printf(1, "running");
    }
    if (state == 5){
        printf(1, "zombie");
    }
    printf(1, " \t");
    return 0;
}


// prints the processInfo of pid
int 
print_process_info(int pid, struct processInfo* p_info)
{
    printf(1, "%d\t", pid);
    print_state(p_info->state);
    printf(1, "%d\t", p_info->ppid);
    printf(1, "%d\t", p_info->sz);
    printf(1, "%d\t", p_info->nfd);
    printf(1, "%d\t", p_info->nrswitch);
    printf(1, "\n");    
    return 0;
}


// runs the main flow
int
main(int argc, char const *argv[])
{
    struct processInfo* p_info = (struct processInfo*) malloc (1*sizeof(struct processInfo)); 
    int num_proc = getNumProc();
    int max_pid = getMaxPid();
    printf(1, "Total number of active processes: %d\n", num_proc);
    printf(1, "Maximum PID = %d\n", max_pid);
    printf(1,"\n");
    printf(1, "PID\tSTATE\t\tPPID\tSZ\tNFD\tNRSWITCH\n");
    for (int pid=1; pid<=max_pid; pid++){
        if (getProcInfo(pid, &p_info) == -1){
            continue;
        }
        print_process_info(pid, p_info);
    }
    free(p_info);
    exit();
}

