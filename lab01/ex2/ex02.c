#include <signal.h>
#include <stdio.h>
#include <unistd.h>


int flag = 1;

static void signalhandler(int sig){

    static int buffer[3] = {0, 0, 0};
    int tmp;

    buffer[2] = buffer[1];
    buffer[1] = buffer[0];
    buffer[0] = sig;

    if(buffer[0] == buffer[1] && buffer[1] == buffer[2]){
        flag = 0;
        printf("3 equal signals received in a row. Exiting...\n");
    }
    else if(buffer[0] == buffer[1]){
        printf("Error: 2 equal signals received. Signo: %d\n", sig);
    }
    else if(buffer[1] != 0){
        printf("Success!\n");
    }

    return;
}

int main(void){

    signal(SIGUSR1, signalhandler);
    signal(SIGUSR2, signalhandler);

    while(flag)
        pause();

    return 0;
}

