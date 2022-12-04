#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>

#ifndef SYS_gettid
#error "SYS_gettid unavailable on this system"
#endif

#define gettid()((pid_t)syscall(SYS_gettid))
#define _GNU_SOURCE

// gcc -Wall -pthread  TP3_1.c -lpthread -o TP3_1 && ./TP3_1

// Fonction appelée par les threads
void *printNumber(int *arg){

    int n = *arg;

    printf("PID % d: %d\n", n, gettid());

    sleep(10);

    // Thread return 10
    return (void *) 10;
}

int main()
{
    int n;
    void *ret;
    
    pthread_t thread1, thread2, thread3;

    printf("Dad PID: %d \n", getpid());

    // Create a thread named thread1
    n = 1;
    if(pthread_create(&thread1, NULL, printNumber, &n) == 0){
        printf("Thread 1 created \n");
    }
    n = 2;
    // Create a thread named thread2
    if(pthread_create(&thread2, NULL, printNumber, &n) == 0){
        printf("Thread 2 created \n");
    }
    n = 3;
    // Create a thread named thread3
    if(pthread_create(&thread3, NULL, printNumber, &n) == 0){
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