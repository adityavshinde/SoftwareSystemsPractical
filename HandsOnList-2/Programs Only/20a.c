/*
============================================================================
Name : 20a.c
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
    int fd = open(fifo, O_WRONLY);
    char message[] = "Hello! writer here";
    write(fd, message, sizeof(message));
    close(fd);
    printf("Message sent: %s\n", message);

    return 0;
}

/*
---------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./20a_out
Message sent: Hello from the writer!
---------------------------------------------------------------------
*/

