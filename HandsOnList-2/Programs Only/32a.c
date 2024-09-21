/*
============================================================================
Name : 32a.c
Author : Aditya Shinde
Description : Write a program to implement semaphore to protect any critical section.
a. rewrite the ticket number creation program using semaphore
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
    char *ticketFile = "/home/g3n1u5/reservation/ticket.txt"; 

    int fd;            
    ssize_t bytesRead, bytesWrote;
    off_t lseekOffset;
    int data; 

    key_t semKey;      
    int semId; 
    int semctlStat;  
    int semopStat;   

    fd = open(ticketFile, O_CREAT | O_RDWR, S_IRWXU);
    if (fd == -1)
    {
        perror("Error while creating / opening the ticket file!");
        _exit(0);
    }

    union semun
    {
        int val;               
        struct semIdentifier_ds *buf;  
        unsigned short *array; 
        struct seminfo *__buf; 
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

        semSet.val = 1; 
        semctlStat = semctl(semId, 0, SETVAL, semSet);
        if (semctlStat == -1)
        {
            perror("Error while initializing a binary sempahore!");
            _exit(1);
        }
    }

    struct sembuf semOp; 
    semOp.sem_num = 0;
    semOp.sem_flg = 0;

    printf("Press enter to obtain lock on the critical section\n");
    getchar();

    semOp.sem_op = -1;
    semopStat = semop(semId, &semOp, 1);
    if (semopStat == -1)
    {
        perror("Error while operating on semaphore!");
        _exit(1);
    }
    
    printf("Obtained lock on critical section!\n");
    printf("Now entering critical section!\n");


    bytesRead = read(fd, &data, sizeof(data));
    if (bytesRead == -1)
    {
        perror("Error while reading from ticket file!");
        _exit(0);
    }
    else if (bytesRead == 0) 
        data = 1;
    else
    {
        data += 1;
        lseekOffset = lseek(fd, 0, SEEK_SET);
        if (lseekOffset == -1)
        {
            perror("Error while seeking!");
            _exit(0);
        }
    }

    bytesWrote = write(fd, &data, sizeof(data));
    if (bytesWrote == -1)
    {
        perror("Error while writing to ticket file!");
        _exit(1);
    }

    printf("Your ticker number is - %d\n", data);

    printf("Press enter to exit from critical section!\n");
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
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./32a_out
Press enter to obtain lock on the critical section

Obtained lock on critical section!
Now entering critical section!
Your ticker number is - 1
Press enter to exit from critical section!

---------------------------------------------------------------------
*/
