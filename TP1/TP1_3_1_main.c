#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

// gcc -Wall TP1_3_1_main.c -o TP1_3_1_main && ./TP1_3_1_main

void Traite_Sig(int);

int main(int argc, char *argv[])
{

    printf("Le PID du process est %d\n", (int)getpid()); // On récupère le PID du processus

    // On vérifie les signaux
    for (int i = 1; i < 32; i++)
    {
        if (signal(i, Traite_Sig) == SIG_ERR)
        {
            printf("Erreur lors de la gestion du signal %d\n", i);
        }
    }

    // Boucle infinie
    while (1)
    {
        sleep(1);
    }

    return 0;
}

// Fonction de traitement des signaux
void Traite_Sig(int signum)
{
    int checkExit = 0;
    // Affichage du signal
    printf("Fonction Traite_Sig : j’ai reçu le signal %d : %s ! \n", signum, strsignal(signum));
}