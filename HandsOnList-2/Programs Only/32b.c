/*
============================================================================
Name : 32b.c
Author : Aditya Shinde
Description : Write a program to implement semaphore to protect any critical section.
b. protect shared memory from concurrent write access
Date: 21st Sep, 2024.
============================================================================
*/
#include <sys/ipc.h>   
#include <sys/sem.h>   
#include <sys/types.h> 
#include <sys/shm.h>   
#include <unistd.h>    
#include <stdio.h>     

void main()
{
    key_t semKey;      
    int semId; 
    int semctlStat;  
    int semopStat;   

    key_t shmKey;         
    int shmId;    
    ssize_t shmSize = 20; 
    char *shmPointer;

    union semun
    {
        int val;               
        struct semIdentifier_ds *buf; 
        unsigned short *array; 
        struct seminfo *__buf; 
    } semSet;

    semKey = ftok(".", 3321);
    if (semKey == -1)
    {
        perror("Error while computing key!");
        _exit(1);
    }

    semId = semget(semKey, 1, 0); 
    if (semId == -1)
    {
        semId = semget(semKey, 1, IPC_CREAT | 0700); 
        if (semId == -1)
        {
            perror("Error while creating semaphore!");
            _exit(1);
        }

        semSet.val = 1; 
        semctlStat = semctl(semId, 0, SETVAL, semSet);
        if (semctlStat == -1)
        {
            perror("Error while initializing a binary sempahore!");
            _exit(1);
        }
    }

    shmKey = ftok(".", 3322);

    if (shmKey == -1)
    {
        perror("Error while computing key!");
        _exit(0);
    }

    shmId = shmget(shmKey, shmSize, IPC_CREAT | 0700); 

    if (shmId == -1)
    {
        perror("Error while getting Shared Memory!");
        _exit(0);
    }

    
    shmPointer = shmat(shmId, (void *)0, 0);

    if (*shmPointer == -1)
    {
        perror("Error while attaching address space!");
        _exit(0);
    }

  
    struct sembuf semOp; 
    semOp.sem_num = 0;
    semOp.sem_flg = 0;

    printf("Press enter to lock the critical section!\n");
    getchar();
    
 
    semOp.sem_op = -1;
    semopStat = semop(semId, &semOp, 1);
    if (semopStat == -1)
    {
        perror("Error while operating on semaphore!");
        _exit(1);
    }
    
    printf("Critical Section is now locked!\n");

    printf("Start of the critical section!\n");


    printf("Writing to the shared memory!\n");

    sprintf(shmPointer, "Hello");

    printf("Press enter to read from the shared memory!\n");
    getchar();

    printf("%s\n", shmPointer);

    printf("Press enter to exit the critical section!\n");
    getchar();


    semOp.sem_op = 1;
    semopStat = semop(semId, &semOp, 1);
    if (semopStat == -1)
    {
        perror("Error while operating on semaphore!");
        _exit(1);
    }

    printf("Critical section is now unlocked!\n");
}

/*
---------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./32b_out
Press enter to lock the critical section!

Critical Section is now locked!
Start of the critical section!
Writing to the shared memory!
Press enter to read from the shared memory!

Hello
Press enter to exit the critical section!

Critical section is now unlocked!
---------------------------------------------------------------------
*/
