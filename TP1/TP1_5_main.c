#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

// gcc -Wall TP1_5_main.c -o TP1_5_main && ./TP1_5_main

FILE *f = NULL; // Fichier de logs
void fonc();
void fonc2();

int main (int argc, char *argv[]) {

    int pid = (int) getpid();
    f = fopen("TP1_5.txt", "a+");
    printf("Le PID du process est %d\n", pid); // On récupère le PID du processus

    while(1) {
        signal(SIGUSR1, SIG_IGN);
        for (int i=1; i < 33; i++){
            
        if(signal(i, fonc) != SIGUSR2 && i != SIGUSR2);
        else if (signal(i, fonc2) == SIGUSR2 && i == SIGUSR2);          
    }
        sleep(10);
        printf("Je dors\n"); 
    }

    fclose(f);
    return 0;
}

void fonc(int signum) {

    int checkExit = 0; 
    f = fopen("TP1_5.txt", "a+");
    printf("Signal %d : %s ! \n", signum, strsignal(signum));
    fprintf(f, "Signal %d : %s ! \n", signum, strsignal(signum));

    printf("Tapez 1 pour quitter !\n");
    scanf( "%d", &checkExit);
    if (checkExit == 1){
        exit(1);
    }
}

void fonc2(int signum){

    int checkExit = 0; 
    f = fopen("TP1_5.txt", "a+");
    printf("J'ai reçu le signal !\n");
    fprintf(f, "J'ai reçu le signal !\n");

    /*
    printf("Tapez 1 pour quitter !\n");
    scanf( "%d", &checkExit);
    if (checkExit == 1){
        exit(1);
    }
    */

}

// Question : envoyer un signal au programme lorsqu'il dort équivaut à le réveiller. Il sort alors du sommeil dans lequel il a été plongé par sleep.