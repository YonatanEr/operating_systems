#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char const *argv[])
{
    getprio();
    ps();
    long num = 1 << 28;
    int pid = fork();
    if (pid==-1){
        printf(1, "\nError in fork\n");
        exit();
    }
    if (pid==0){
        setprio(7);
        ps();
        for (volatile long l1=0; l1<num; l1++){
            l1++;
            l1--;
        }
        printf(1, "\nChild exit\n");
        exit();
    }
    else{ 
        setprio(6);
        ps();
        for (volatile long l2=0; l2<num; l2++){
            l2++;
            l2--;
        }
        printf(1, "\nParent is waiting for child\n");
        wait();
    }
    ps();
    printf(1, "Parent exit\n");
    exit();
}

