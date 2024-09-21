/*
============================================================================
Name : 20b.c
Author : Aditya Shinde
Description : Write two programs so that both can communicate by FIFO -Use one way communication.
Date: 19th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    char *fifo = "/tmp/myfifo";
    mkfifo(fifo, 0666);
    int fd = open(fifo, O_RDONLY);
    char buffer[100];
    read(fd, buffer, sizeof(buffer));
    printf("Message received: %s\n", buffer);
    close(fd);

    return 0;
}

/*
---------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./20b_out
Message received: Hello from the writer!
---------------------------------------------------------------------
*/

