#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

// Compiler : gcc -Wall TP1_2_2_main.c -o TP1_2_2_main && ./TP1_2_2_main

FILE *f = NULL; // Fichier de logs
void sighandler(int); // Fonction de gestion des signaux

int main (int argc, char *argv[]) {

    f = fopen("TP1_2_2.txt", "a+"); // Ouverture du fichier de logs
    printf("Le PID du process est %d\n", (int) getpid ()); // On récupère le PID du processus

    // 3) Ajout de la boucle infinie
    while(1) {

        // On vérifie les signaux
        for (int i=1; i < 32; i++){
            if(signal(i, SIG_IGN) == SIG_ERR){
                printf("Erreur lors de la gestion du signal %d\n", i);
            }
        }

        sleep(1); // Sleep pour tester les signaux à envoyer au programme.
    }

    fclose(f); // Fermeture du fichier de logs
    return 0;
}

void sighandler(int signum) {

    int checkExit = 0; // Variable de sortie de boucle (pour fermer le programme)
    f = fopen("TP1_2_2.txt", "a+");
    printf("Signal non ignoré %d : %s ! \n", signum, strsignal(signum)); // Affichage du signal
    fprintf(f, "Signal %d non ignoré : %s ! \n", signum, strsignal(signum)); // Ecriture dans le fichier de logs

    printf("Tapez 1 pour quitter !\n");
    scanf( "%d", &checkExit);

    // Test pour quitte le programme
    if (checkExit == 1){
        exit(1);
    }
    
}

// 2) Les signaux ne sont pas ignorables car il s'agit d'événements systèmes ? Notamment le changement de fenêtres.

/* 
    4) Quand on fait crtl C on envoie un signal d'interruption au système, mais le programme continu puisqu'il s'exécute dans une boucle infinie 
        Le signal kill -9 PID a un effet sur le programme --> ça le kill de manière brutale. Sans le signal -9, ça crée juste un événement mais ça ne le tue pas.
        Le changement de fenêtre a également un effet visible sur le programme. 

        // Tester avec d'autres trucs pour voir si je peux le kill/sleep et le réveiller
        // Kill -l pour lister les signaux

*/


