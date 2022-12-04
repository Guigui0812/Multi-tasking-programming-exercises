#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <semaphore.h>

#ifndef SYS_gettid
#error "SYS_gettid unavailable on this system"
#endif

#define gettid() ((pid_t)syscall(SYS_gettid))
#define _GNU_SOURCE

#define LIMITE_CLIENT 8

// Données par partagées entre les threads
typedef struct s_threadData
{
    int nbClientsAttente, cptClients;
} ThreadData;

// Sémaphores positionnés
sem_t coiffeur; // sémapohore pour le coiffeur
sem_t client;   // sémapohore pour les clients
sem_t mutexCpt; // mutex des compteurs

// gcc -Wall -pthread  Coiffeur.c -lpthread -o Coiffeur && ./Coiffeur

// Fonction du coiffeur
void *coiffe(void *args)
{
    // Tant que le salon est ouvert
    while (1)
    {
        ThreadData *data = args;

        // Si il n'y a pas de clients le coiffeur dort.
        if (data->nbClientsAttente == 0)
        {
            printf("Le coiffeur dort\n");
        }

        // Il attend un client
        sem_wait(&client);

        // Le client se fait coiffer
        printf("Coiffeur : Je coupe les cheveux\n");
        int random = (rand() % (6 - 4 + 1)) + 4;
        sleep(random);

        // Maj du compteur de clients en attente protégé par le mutex
        sem_wait(&mutexCpt);
        data->nbClientsAttente--;
        sem_post(&mutexCpt);

        // Le coiffeur est libre de prendre un nouveau client
        sem_post(&coiffeur);
    }
}

// Fonction des clients
void *Client(void *args)
{
    ThreadData *data = args;
    int nbClient = data->cptClients;

    // Le client attend la libération du coiffeur
    sem_wait(&coiffeur);
    // Le client attend de passer chez le coiffeur
    sem_post(&client);
    printf("Le client a été coiffé et part.\n");
}

int main()
{
    // Initialisation des sémaphores
    sem_init(&client, 0, 0);
    sem_init(&coiffeur, 0, 1);
    sem_init(&mutexCpt, 0, 1);

    int nbSiegesMax = 3;
    int i = 0;
    ThreadData data;
    data.nbClientsAttente = 0;
    data.cptClients = 0;
    pthread_t coiffeur, client;

    // Création du coiffeur
    if (pthread_create(&coiffeur, NULL, coiffe, &data) == 0)
    {
        printf("Le coiffeur est ouvert\n");
    }

    // Tant que l'on décide de garder le salon ouvert (ctrl c)
    while (1)
    {
        // Generate a random between 1 and 3
        int random = rand() % 3 + 1;

        // Sleeps pour plus de confort
        sleep(random);

        if (data.cptClients < LIMITE_CLIENT)
        {
            // Si il reste des places dans le salon pour attendre
            if (data.nbClientsAttente < nbSiegesMax)
            {
                // On crée un nouveau thread client            
                if (pthread_create(&client, NULL, Client, &data) == 0)
                {
                    printf("Un client arrive et attend d'être coiffé. \n");
                    i++;
                    // Baisser le sémaphore de nbClientsAttente
                    sem_wait(&mutexCpt);
                    data.nbClientsAttente++;
                    data.cptClients++;
                    sem_post(&mutexCpt);
                }
            }
            else // Si plus de places le client repart
            {
                printf("Le salon est plein, le client repart\n");
            }
        }
    }

    return 0;
}