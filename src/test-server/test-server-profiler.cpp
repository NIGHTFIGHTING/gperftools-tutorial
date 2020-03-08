#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include <google/profiler.h>

//SIGUSR1: start profiling
//SIGUSR2: stop profiling

static void gprof_callback(int signum) {
    if (signum == SIGUSR1) {
        printf("Catch the signal ProfilerStart\n");
        ProfilerStart("test-server.prof");
    }
    else if (signum == SIGUSR2) {
        printf("Catch the signal ProfilerStop\n");
        ProfilerStop();
    }
}

static void setup_signal() {
    struct sigaction profstat;
    profstat.sa_handler = gprof_callback;
    profstat.sa_flags = 0;
    sigemptyset(&profstat.sa_mask);                                        
    sigaddset(&profstat.sa_mask, SIGUSR1);
    sigaddset(&profstat.sa_mask, SIGUSR2);

    if (sigaction(SIGUSR1, &profstat,NULL) < 0) 
        fprintf(stderr, "SIGUSR1 Fail !");

    if (sigaction(SIGUSR2, &profstat,NULL) < 0) 
        fprintf(stderr, "SIGUSR2 Fail !");
}

int loopop_callee() {
    int n=0;
    for(int i=0; i<10000; i++) 
        for(int j=0; j<10000; j++)
            n |= i%100 + j/100;
    return n;
}

int loopop() {
    int n=0;
    while(1) {
        for(int i=0; i<10000; i++) 
            for(int j=0; j<10000; j++) 
                n |= i%100 + j/100;
        printf("result:  %d\n", (loopop_callee)() );
    }
    return n;
}

int main(int argc,char** argv) {
    setup_signal();
    printf("result:  %d\n", (loopop)() );
    return 0;
}
