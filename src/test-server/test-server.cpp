#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>


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
    printf("result:  %d\n", (loopop)() );
    return 0;
}
