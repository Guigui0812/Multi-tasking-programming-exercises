#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

// gcc -Wall TP1_4_1_main.c -o TP1_4_1_main && ./TP1_4_1_main

void fonc();
void fonc1();
void fonc2();

int main(int argc, char *argv[])
{

    int pid = (int)getpid();
    printf("Le PID du process est %d\n", pid); // On récupère le PID du processus

    // On vérifie les signaux
    for (int i = 1; i < 32; i++)
    {
        if (i == SIGUSR1){
            if (signal(i, fonc1) == SIG_ERR)
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
        sleep(1);
    }

    return 0;
}

void fonc(int signum)
{
    // Affiche le numéro du signal reçu
    int checkExit = 0;
    printf("Signal %d : %s ! \n", signum, strsignal(signum));
}

void fonc1(int signum)
{
    // fonc1 affiche le numéro du signal reçu et la liste des utilisateurs de la machine
    int checkExit = 0;
    printf("Signal reçu %d : %s ! \n Utilisateurs : %d \n", signum, strsignal(signum), system("who"));
}

void fonc2(int signum)
{

    // fonc2 affiche le numéro du signal reçu et l'espace disque utilisé sur la machine (appel à df . par system("df .")).
    int checkExit = 0;
    printf("Signal reçu %d : %s ! \n Utilisateurs : %d \n", signum, strsignal(signum), system("df ."));
}