#define _POSIX_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/wait.h>

// gcc -Wall Star_Wars.c -o Star_Wars && ./Star_Wars

void Luke();
void Vador();
void Attack();

// Variable globale des processus
int PV = 8;
int degats = 0;
char str[100]; // nom des protagonistes
int cible;     // cible du coup
int tpsAttack; // temps d'attaque

// Deux process : vador / luke
// Si l'un envoi SIGUSR1 vers l'autre --> engendre une fonction typique

// Fonction d'attaque
void Attack()
{
    printf("%s est attaqué !\n", str);
    PV = PV - degats;
}

int main(int argc, char *argv[])
{
    int dieu = (int)getpid();
    int vador = fork(); // Anakin devient vador...
    int luke;
   
    // Vador combat
    if (vador == 0)
    {
        // Setup des valeurs de Vador
        strcpy(str, "Vador"); // Nom du combattant
        degats = 2; // Dégats reçus par le combattant
        tpsAttack = 3; // Temps d'attaque
        printf("Luke est né ! \n");
        luke = fork(); // Vador a un fils...
        cible = luke; // Cible des attaques du combattant

        if (luke == 0)
        {
            // Setup des valeurs de luke
            strcpy(str, "Luke");
            degats = 3;
            tpsAttack = 2;
            cible = getpid() - 1;
            srand(time(NULL));      
        }
       
        // Le destin de l'univers se joue...
        while (PV > 0)
        {
            int random = rand() % 5 + 1;
            signal(SIGUSR1, SIG_IGN); // En garde...
            sleep(random);            // ...pendant "random" secondes
            signal(SIGUSR1, Attack);  // Il baisse sa garde pour attaquer
            sleep(tpsAttack);         // vulnérable pendant "tpsAttack" secondes

            kill(cible, SIGUSR1); // Envoie du signal d'attaque

            signal(SIGUSR1, SIG_IGN); // De nouveau en garde
        }

        // Si un combbattant n'a plus de PV
        if (PV <= 0)
        {
            printf("%s est mort !\n", str);
            kill(cible, SIGKILL); // Kill de sa cible
            exit(0); // On quitte proprement
        }
    }
    else{
        wait(NULL); // On attend le processus fils
    }

    return 0;
}