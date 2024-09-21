/*
============================================================================
Name : 21a.c
Author : Aditya Shinde
Description : Write two programs so that both can communicate by FIFO -Use two way communications.
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char msg1[] = "Hello from Program 21a";
    char msg2[100];

    mkfifo("fifo1", 0666);
    mkfifo("fifo2", 0666);

    write(open("fifo1", O_WRONLY), msg1, sizeof(msg1));
    read(open("fifo2", O_RDONLY), msg2, sizeof(msg2));

    printf("I received: %s\n", msg2);

    return 0;
}

/*
----------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./21a_out
I received: Hello from Program 21b
----------------------------------------------------------------------
*/
