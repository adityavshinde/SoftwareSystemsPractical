#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char msg1[100];
    char msg2[] = "Hello from Program 21b";

    mkfifo("fifo1", 0666);
    mkfifo("fifo2", 0666);

    read(open("fifo1", O_RDONLY), msg1, sizeof(msg1));
    write(open("fifo2", O_WRONLY), msg2, sizeof(msg2));

    printf("I received: %s\n", msg1);

    return 0;
}

/*
----------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./21b_out
I received: Hello from Program 21a
----------------------------------------------------------------------
*/
