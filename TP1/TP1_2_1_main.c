#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

// gcc -Wall TP1_2_1_main.c -o TP1_2_1_main && ./main

int main (int argc, char *argv[]) {

    FILE *f = fopen("TP1_2_1.txt", "a+");
    // Boucle sur les signaux
    for (int i = 0; i < _NSIG; i++){
        fprintf(f, "Signaux = %s \n", strsignal(i));
    }

    fclose(f);
    return 0;
}

// On peut identifier 31 signaux utiles, qui ont une réelle utilité pour le système. 