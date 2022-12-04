#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

// gcc -Wall TP2.c -o TP2 && ./TP2

int main (int argc, char *argv[]) {

    int tube[2];
    char string[100];
    int n;
    int pid;
    pipe(tube);
    pid = fork();

    if(pid == 0){
        close(tube[1]);
        while((n = read(tube[0], string, 100))){
            write(1, string, n);
        }
        
        close(tube[0]);
        
        exit(EXIT_SUCCESS);
    }
    else{
        close(tube[0]);
        while((n = read(0, string, 100)) > 0){
            write(tube[1], string, n);
        }
        close(tube[1]);
        wait(NULL); 
        printf("Mon fils est mort \n");     
        exit(EXIT_SUCCESS);
    }

    return 0;
}