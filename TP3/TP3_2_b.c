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

#define gettid()((pid_t)syscall(SYS_gettid))
#define _GNU_SOURCE

sem_t mutex;

// gcc -Wall -pthread  TP3_2_b.c -lpthread -o TP3_2_b && ./TP3_2_b

// Structure de donnée des threads
typedef struct s_threadData {
    int a, b;
} ThreadData ;

// Fonction appelée par les threads
void *threadFunc(void *args){

    /*
    Attendre entre 0 et 1 seconde (temps aléatoire)
    - Ajouter 1 à a,
    - Attendre entre 0 et 1 seconde (temps aléatoire)
    - Ajouter 1 à b,
    - Attendre entre 0 et 1 seconde (temps aléatoire)
    - Afficher le résultat sous la forme : « Thread1 : a / b »
    */

    sem_wait(&mutex); // Debut de la zone critique

    sleep(rand() % 2);

    ThreadData* data = args;
    data->a +=1;

    // Sleep the thread between 0 and 1 second
    sleep(rand() % 2);

    data->b +=1;

    sleep(rand() % 2);

    printf("Thread 1 : %d / %d\n", data->a, data->b);

    sem_post(&mutex); // Fin de la zone critique
    // Thread return 10
    return (void *) 10;
}

// Fonction appelée par les threads
void *threadFunc2(void *args){

    /*
    - Attendre entre 0 et 1 seconde (temps aléatoire)
    - Multiplier par 2 la variable a,
    - Attendre entre 0 et 1 seconde (temps aléatoire)
    - Multiplier par 2 la variable b,
    - Attendre entre 0 et 1 seconde (temps aléatoire)
    - Afficher le résultat sous la forme : « Thread2 : a / b »
    */

    sem_wait(&mutex); // Debut de la zone critiqu
    sleep(rand() % 2);

    ThreadData* data = args;
    data->a *=2;

    // Sleep the thread between 0 and 1 second
    sleep(rand() % 2);

    data->b *=2;

    sleep(rand() % 2);

    printf("Thread 2 : %d / %d\n", data->a, data->b);

    sem_post(&mutex); // Fin de la zone critique

    // Thread return 10
    return (void *) 10;
}

// Fonction appelée par les threads
void *threadFunc3(void *args){

    /*
    - Attendre entre 0 et 1 seconde (temps aléatoire)
    - soustraire 1 à a,
    - Attendre entre 0 et 1 seconde (temps aléatoire)
    - soustraire 1 à b,
    - Attendre entre 0 et 1 seconde (temps aléatoire)
    - Afficher le résultat sous la forme : « Thread3 : a / b »
    */

    sem_wait(&mutex); // Debut de la zone critiqu
    sleep(rand() % 2);

    ThreadData* data = args;
    data->a -=1;

    // Sleep the thread between 0 and 1 second
    sleep(rand() % 2);

    data->b -=1;

    sleep(rand() % 2);

    printf("Thread 3 : %d / %d\n", data->a, data->b);

    sem_post(&mutex); // Fin de la zone critique

    // Thread return 10
    return (void *) 10;
}

int main()
{
    // Declare a threadData Struct
    ThreadData data;
    pthread_t thread1, thread2, thread3;
    data.a = 1;
    data.b = 1;
    void *ret;
    sem_init(&mutex, 0, 1); // Init du mutex

    printf("Dad PID: %d \n", getpid());

    // Create a thread named thread1
    if(pthread_create(&thread1, NULL, threadFunc, &data) == 0){
        printf("Thread 1 created \n");
    }
    // Create a thread named thread2
    if(pthread_create(&thread2, NULL, threadFunc2, &data) == 0){
        printf("Thread 2 created \n");
    }
    // Create a thread named thread3
    if(pthread_create(&thread2, NULL, threadFunc3, &data) == 0){
        printf("Thread 3 created \n");
    }
    
    printf("Waiting for thread 1...\n");
    pthread_join(thread1, (void**) &ret);
    printf("Thread 1, exit code: %d\n", ret);
    
    printf("Waiting for thread 2...\n");
    pthread_join(thread2, (void**) &ret);
    printf("Thread 2, exit code: %d\n", ret);
    
    printf("Waiting for thread 3...\n");
    pthread_join(thread3, (void**) &ret);
    printf("Thread 3, exit code: %d\n", ret);

    return 0;
}