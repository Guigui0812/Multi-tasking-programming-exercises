#define _POSIX_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <setjmp.h>
#include <sys/types.h>

// gcc -Wall TP1_6.c -o TP1_6 && ./TP1_6

sigjmp_buf buf;

void fonc();
void fonc2();
void Traite_FPE();

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
        else if (i == SIGFPE){
            if (signal(i, Traite_FPE) == SIG_ERR)
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
        
        if (sigsetjmp(buf, 0) != 0)
	    printf("Reprise sur erreur\n");
    	printf("Attention à la division par 0 !\n");
        sleep(1);
        int test = 2 / 0;
    }

    return 0;
}

void Traite_FPE(){
    printf("Détection d'une erreur\n");
    siglongjmp(buf, -1);
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

