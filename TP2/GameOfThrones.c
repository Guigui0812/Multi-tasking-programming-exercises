#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

// Pour compiler : gcc -Wall GameOfThrones.c -o GameOfThrones && ./GameOfThrones

// Dans le sujet, il est mentionné l'existence de morts-vivants et d'un marcheur blanc.
// J'ai pris la liberté de renommer les morts-vivants des "marcheurs blancs" et de renommer leur chef le Roi de la Nuit. 
// C'est comme ça dans la saga écrite par George R.R. Martin...

#define NB_MARCHEURS_BLANCS 5

int undeadStatus; // Statut du mort vivant : mort ou vivant

void undeadDead() // Fonction qui est appelé dès qu'un mort vivant est tué par Jon Snow avec SIGURS2
{
    printf("Jon Snow a tué le mort vivant matricule %d envoyé par le Night King !\n", getpid());
    undeadStatus = 0;
}

int main(int argc, char *argv[])
{
    int tube[2]; // Tube utilisé pour transmettre les infos entre le Night King (Roi de la Nuit) et Jon Snow
    pipe(tube); // Init du tube

    // Generation d'un random entre 0 ou 1 pour décider qui gagnera le combat à la fin.
    srand(time(NULL));
    int winner = rand() % 2;
 
    // Naissance de Jon Snow
    int jonSnow = fork();

    if (jonSnow == 0) // Code du process Jon Snow
    {
        int cpt = 0; // Cpt qui compte le nombre de morts vivants tués...
        int undeadPid, undeadPIDtmp; // PID des marcheurs blancs + variable tmp pour retenir le PID du Night King

        // Tant que Jon Snow n'a pas tué tous les marcheurs blancs, il continue de tuer des morts vivants
        while (cpt < NB_MARCHEURS_BLANCS)
        {
            sleep(1); // Il se repose entre deux coups !
            read(tube[0], &undeadPid, sizeof(int)); // Il lit les infos que le Night King lui envoie dans le tube
            undeadPIDtmp = undeadPid; // Il récupère le PID du dernier marcheur blanc qu'il a tué
            if (kill(undeadPid, SIGUSR2) == 0)
            {
                cpt++; // Quand il kill un marcheur blanc, il incrémente le cpt
            }
        }

        if(winner == 1) // Si c'est à lui de gagner le combat
        {
            printf("Jon Snow a gagné !\n"); // Il gagne le combat
            kill(undeadPIDtmp - NB_MARCHEURS_BLANCS, SIGKILL); // Jon utilise le matricule des marcheurs pour trouver leur père : le Night King
            exit(0); // Il peut mourir en paix
        }   
    }
    else // Dieu
    {
        // Dieu crée le Night King (le père des morts-vivants, ou plutôt des marcheurs blancs si on reste chez le bon George R.R. Martin)
        int nightKing = fork();

        // Code du Roi de la Nuit
        if (nightKing == 0)
        {
            int pid, status, wpid; // PID des marcheurs blancs, status des marcheurs blancs, wpid des marcheurs blancs
            
            int newWhitWalker; // PID du nouveau marcheur blanc

            // Tant que le Night King n'a pas envoyé tous ses marcheurs blancs, il continue d'en envoyer
            for (int i = 0; i < NB_MARCHEURS_BLANCS; i++)
            {          
                if ((newWhitWalker = fork()) == 0) // Si le marcheur est bien né
                {
                    undeadStatus = 1; // Le marcheur blanc est vivant, pour l'instant...
                    signal(SIGUSR2, undeadDead); // initialisation du handler du marcheur blanc
                    int undeadPID = getpid(); // On récupère le PID du marcheur
                    printf("Le marcheur blanc matricule %d se dirige vers Jon !\n", getpid()); // Le marcheur est envoyé vers Jon
                    write(tube[1], &undeadPID, sizeof(int)); // On écrit son PID dans le tube pour que Jon puisse le tuer

                    // Tant que Jon ne le tue pas, le marcheur est vivant !
                    while (1) 
                    {
                        // Si Jon tape le marcheur avec SIGUSR2, le marcheur meurt !
                        if (undeadStatus == 0)
                        {
                            exit(0);
                        }
                    }
                } 
            }

            // Le Night King attend que tous ses marcheurs soient morts avant d'attaquer.
            while ((wpid = wait(&status)) > 0); 

            // Si le random déterminant l'issue du combat donne le Night King vainqueur !
            if(winner == 0)
            {
                printf("Le Night King a gagné !\n"); // Le Night King gagne le combat
                kill(jonSnow, SIGKILL); // Jon Snow est tué
                exit(0); // Le Night King a fini sa mission et peut mourir en paix
            }
        }
        else{
            wait(NULL); // Dieu attend ses enfants.
        }
    }
}