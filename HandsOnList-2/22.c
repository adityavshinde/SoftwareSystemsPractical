#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>

int main() {
    char buffer[100];
    int fd = open("fifofile", O_RDONLY | O_NONBLOCK);  
    fd_set readfds;
    struct timeval timeout = {10, 0}; 

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    if (select(fd + 1, &readfds, NULL, NULL, &timeout) > 0) {
        read(fd, buffer, sizeof(buffer));
        printf("Data received: %s\n", buffer);
    } else {
        printf("Timeout: No data received within 10 seconds.\n");
    }

    close(fd);
    return 0;
}

/*
---------------------------------------------------------------------------------------
OUTPUT:
Terminal 1:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2/22$ ./22_out

After executing this, in other terminal we should write the message:
Terminal 2:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2/22$ echo "Hello" > fifofile

Terminal 1:
Data received: Hello

---------------------------------------------------------------------------------------
*/
