#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

// Compiler : gcc -Wall TP1_2_3.c -o TP1_2_3 && ./TP1_2_3

void sighandler(int); // Fonction de gestion des signaux

int main (int argc, char *argv[]) {

    printf("Le PID du process est %d\n", (int) getpid ()); // On récupère le PID du processus

        // On vérifie que les signaux sont bien gérés
        for (int i=1; i < 32; i++){
            if(signal(i, SIG_IGN) == SIG_ERR){
                printf("Erreur lors de la gestion du signal %d\n", i);
            }
        }

    while (1) {
        sleep(1);
    }

    return 0;
}