#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <semaphore.h>

// gcc -Wall -pthread  Coiffeurs.c -lpthread -o Coiffeurs && ./Coiffeurs

#ifndef SYS_gettid
#error "SYS_gettid unavailable on this system"
#endif

#define gettid() ((pid_t)syscall(SYS_gettid))
#define _GNU_SOURCE

#define LIMITE_CLIENT 15

// Données par partagées entre les threads
typedef struct s_threadData
{
    int nbClientesAttente, nbClientsAttente, cptClients, cptClientes, nbSiegesMax, cptClientGlobal;
} ThreadData;

// Sémaphores positionnés
sem_t coiffeur;        // sémapohore pour le coiffeur
sem_t client, cliente; // sémapohore pour les clients
sem_t cpt;             // mutex des compteurs

// Fonction du coiffeur pour hommes
void *coiffeHomme(void *args)
{
    // Tant que le salon est ouvert
    while (1)
    {
        ThreadData *data = args;

        // Si il n'y a pas de clients le coiffeur dort.
        if (data->nbClientesAttente == 0 || data->nbClientsAttente == 0)
        {
            printf("Le coiffeur Homme dort dans le fauteuil\n");
            sleep(4);
        }

        // Si des clients hommes sont présents
        if (data->cptClientGlobal > 0)
        {
            // Cas où il y a des clients et des clientes
            if (data->nbClientsAttente > 0)
            {
                // Le coiffeur attend d'avoir un client
                sem_wait(&client);

                printf("Coiffeur : Je coupe les cheveux d'homme\n");

                // On attend pendant un nombre aléatoire de secondes

                // Generate a random int in 4 to 6
                int random = (rand() % (6 - 4 + 1)) + 4;
                sleep(random);

                // Mise à jour des compteurs protégés par le mutex
                sem_wait(&cpt);
                data->nbClientsAttente--;
                data->cptClientGlobal--;
                sem_post(&cpt);
            }
            else // Cas où il n'y a que des clientes
            {

                sem_wait(&cliente);
                printf("Coiffeur : Je coupe exceptionnellement les cheveux d'une femme\n");

                // On attend pendant un nombre aléatoire de secondes
                int random = (rand() % (6 - 4 + 1)) + 4;
                sleep(random);

                // Mise à jour des compteurs protégés par le mutex
                sem_wait(&cpt);
                data->nbClientesAttente--;
                data->cptClientGlobal--;
                sem_post(&cpt);
            }
        }

        // On lève le sémaphore
        sem_post(&coiffeur);
    }
}

// Fonction du coiffeur pour femmes
void *coiffeFemme(void *args)
{
    // Tant que le salon est ouvert
    while (1)
    {
        ThreadData *data = args;

        // Si il n'y a pas de clients le coiffeur dort
        if (data->nbClientesAttente == 0 || data->nbClientsAttente == 0)
        {
            printf("Le coiffeur Femme dort dans le fauteuil\n");
            sleep(4);
        }

        // Si des clients sont présents
        if (data->cptClientGlobal > 0)
        {
            // Cas où il y a des clients et des clientes
            if (data->nbClientesAttente > 0)
            {
                // On libère la cliente
                sem_wait(&cliente);
                printf("Coiffeur : Je coupe les cheveux d'un femme\n");

                // On attend pendant un nombre aléatoire de secondes
                int random = (rand() % (6 - 4 + 1)) + 4;
                sleep(random);

                // Maj des compteurs protégés par le mutex
                sem_wait(&cpt);
                data->nbClientesAttente--;
                data->cptClientGlobal--;
                sem_post(&cpt);
            }
            else // Cas où il n'y a que des clients
            {
                // Le coiffeur s'occupe exceptionnellement d'un homme
                sem_wait(&client);
                printf("Coiffeur : Je coupe exceptionnellement les cheveux d'un homme\n");

                // La coupe dure une certaine durée aléatoire
                int random = (rand() % (6 - 4 + 1)) + 4;
                sleep(random);

                // Maj des compteurs protégés par le mutex
                sem_wait(&cpt);
                data->nbClientsAttente--;
                data->cptClientGlobal--;
                sem_post(&cpt);
            }
        }

        // Levée du sémaphore
        sem_post(&coiffeur);
    }
}

// Fonction des clients
void *Cliente(void *args)
{

    ThreadData *data = args;
    int nbClient = data->cptClients;

    // Le client indique son arrivée
    sem_post(&cliente);

    // Attend que le coiffeur le coiffe et le libère
    sem_wait(&coiffeur);

    printf("La cliente a été coiffée et part.\n");
}

// Fonction des clients
void *Client(void *args)
{
    ThreadData *data = args;
    int nbClient = data->cptClients;

    // Le client indique son arrivée
    sem_post(&client);

    // Attend que le coiffeur le coiffe et le libère
    sem_wait(&coiffeur);

    printf("Le client a été coiffé et part.\n");
}

int main()
{
    // Init des sémaphores
    sem_init(&client, 0, 0);
    sem_init(&cliente, 0, 0);
    sem_init(&coiffeur, 0, 0);
    sem_init(&cpt, 0, 1);

    pthread_t coiffeurHomme, coiffeurFemme, client, cliente;

    // Init des divers compteurs et seuils
    ThreadData data;
    data.nbSiegesMax = 3;
    data.nbClientsAttente = 0;
    data.cptClients = 0;
    data.cptClientes = 0;
    data.nbClientesAttente = 0;
    data.cptClientGlobal = 0;

    int cptGlobal = 0;

    // Création des coiffeurs
    if (pthread_create(&coiffeurFemme, NULL, coiffeFemme, &data) == 0)
    {
        printf("Le coiffeur femme est ouvert\n");
    }
    if (pthread_create(&coiffeurHomme, NULL, coiffeHomme, &data) == 0)
    {
        printf("Le coiffeur homme est ouvert\n");
    }

    while (1)
    {

        // Sleep pour plus de confort
        int random = (rand() % (3 - 1 + 1)) + 1;
        sleep(random);

        printf("Nombre de personnes dans la salle d'attente : %d \n", data.cptClientGlobal);

        if (cptGlobal < LIMITE_CLIENT)
        {

            if (data.cptClientGlobal < data.nbSiegesMax)
            {

                // Génération d'un nombre aléatoire pour déterminer le sexe du client
                int random2 = rand() % 2;

                // Un homme arrive si 0, une femme si 1
                if (random2 == 0)
                {
                    printf("Un homme arrive, il s'assoit dans un fauteuil. \n");
                    pthread_create(&client, NULL, Client, &data);
                    sem_wait(&cpt);
                    data.nbClientsAttente++;
                    data.cptClients++;
                    sem_post(&cpt);
                }
                else
                {
                    printf("Une femme arrive, elle s'assoit dans un fauteuil. \n");
                    pthread_create(&client, NULL, Cliente, &data);
                    sem_wait(&cpt);
                    data.nbClientesAttente++;
                    data.cptClientes++;
                    sem_post(&cpt);
                }

                data.cptClientGlobal++;
            }
            else
            {
                printf("Le salon est plein, le client repart\n");
            }

            cptGlobal++;
        }
    }

    return 0;
}