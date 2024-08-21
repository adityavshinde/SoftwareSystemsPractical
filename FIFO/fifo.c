#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main() {
    const char *fifofile = "/home/g3n1u5/SS/SoftwareSystemsPractical/FIFO/fifofile.txt";
    if (mkfifo(fifofile, 0666) == -1) {
        perror("mkfifo failed");
    } else {
        printf("FIFO has been created.\n");
    }
    return 0;
}
