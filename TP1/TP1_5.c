#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

// gcc -Wall TP1_5.c -o TP1_5 && ./TP1_5

void fonc();
void fonc2();

int main(int argc, char *argv[])
{

    int pid = (int)getpid();
    printf("Le PID du process est %d\n", pid); // On récupère le PID du processus

    // On vérifie les signaux
    for (int i = 1; i < 32; i++)
    {
        if (i == SIGUSR1){
            if (signal(i, SIG_IGN) == SIG_ERR)
            {
                printf("Erreur lors de la gestion du signal %d\n", i);
            }
        }
        else if (i == SIGUSR2){
            if (signal(i, fonc2) == SIG_ERR)
            {
                printf("Erreur lors de la gestion du signal %d\n", i);
            }
        }
        else{
            if (signal(i, fonc) == SIG_ERR)
            {
                printf("Erreur lors de la gestion du signal %d\n", i);
            }
        }
    }

    while (1)
    {
        sleep(10);
        printf("Je dors\n"); 
    }

    return 0;
}

void fonc(int signum)
{
    // Affiche le numéro du signal reçu
    int checkExit = 0;
    printf("Signal %d : %s ! \n", signum, strsignal(signum));
}

void fonc2(int signum){
    int checkExit = 0; 
    printf("J'ai reçu le signal !\n");
}

