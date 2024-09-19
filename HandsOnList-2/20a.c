#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;
    char *fifo = "fifo_file";
    char message[] = "Hello from writer";

    fd = open(fifo, O_WRONLY);

    write(fd, message, sizeof(message));
    printf("Message has been written");

    close(fd);

    return 0;
}

