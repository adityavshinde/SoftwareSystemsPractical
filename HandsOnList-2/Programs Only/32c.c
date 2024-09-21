/*
============================================================================
Name : 32c.c
Author : Aditya Shinde
Description : Write a program to implement semaphore to protect any critical section.
c. protect multiple pseudo resources ( may be two) using counting semaphore
Date: 21st Sep, 2024.
============================================================================
*/
#include <sys/ipc.h>   
#include <sys/sem.h>   
#include <sys/types.h> 
#include <sys/stat.h>  
#include <fcntl.h>     
#include <unistd.h>    
#include <stdio.h>     

void main()
{
    char *filename = "/home/g3n1u5/reservation/ipsum.txt"; 

    int fd;            
    ssize_t bytesRead; 
    off_t lseekOffset;
    char data[1000]; 

    key_t semKey;     
    int semId; 
    int semctlStat;  
    int semopStat;  

    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("Error while creating / opening the ticket file!");
        _exit(0);
    }

    union semun
    {
        int val;              
    } semSet;

    semKey = ftok(".", 331);
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

        semSet.val = 2; 
        semctlStat = semctl(semId, 0, SETVAL, semSet);
        if (semctlStat == -1)
        {
            perror("Error while initializing a binary sempahore!");
            _exit(1);
        }
    }

    struct sembuf semOp; 
    semOp.sem_num = 0;
    semOp.sem_flg = IPC_NOWAIT;

    printf("Press enter to obtain lock for the critical section!\n");
    getchar();

    semOp.sem_op = -1;
    semopStat = semop(semId, &semOp, 1);
    if (semopStat == -1)
    {
        perror("Error while operating on semaphore!");
        _exit(1);
    }

    printf("Obtained lock on critical section!");

    printf("Now entering critical section!");


    bytesRead = read(fd, &data, 1000);
    if (bytesRead == -1)
    {
        perror("Error while reading from sample file!");
        _exit(0);
    }
    else if (bytesRead == 0) 
        printf("No data exists!\n");
    else
        printf("Sample data: \n %s", data);

    printf("\n\nPress enter key to exit critical section!\n");
    getchar();


    semOp.sem_op = 1;
    semopStat = semop(semId, &semOp, 1);
    if (semopStat == -1)
    {
        perror("Error while operating on semaphore!");
        _exit(1);
    }

    close(fd);
}

/*
---------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./32c_out
Press enter to obtain lock for the critical section!

Obtained lock on critical section!Now entering critical section!Sample data: 
 Hello! I've written this data.


Press enter key to exit critical section!

---------------------------------------------------------------------
*/
