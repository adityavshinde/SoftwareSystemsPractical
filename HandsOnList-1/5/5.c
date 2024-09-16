/*
============================================================================
Name : 5.c
Author : Aditya Vijay Shinde
Description : Write a program to create five new files with infinite loop. Execute the program in the background and check the file descriptor table at /proc/pid/fd.
Date: 28th Aug, 2024.
============================================================================
*/
#include <fcntl.h>   // For open()
#include <stdio.h>    // For printf() and perror()
#include <unistd.h>   // For close() and sleep()

int main() {
    const char *filenames[] = {"f1.txt", "f2.txt", "f3.txt", "f4.txt", "f5.txt"};
    int fds[5];

    // Create five new files
    for (int i = 0; i < 5; i++) {
        fds[i] = open(filenames[i], O_CREAT | O_WRONLY, 0644);
        if (fds[i] == -1) {
            perror("File creation failed");
            return 1;
        } else {
            printf("File %s created with file descriptor: %d\n", filenames[i], fds[i]);
        }
    }

    // Infinite loop
    while (1) {
        sleep(1);  // To avoid high CPU usage
    }

    // Optionally close files (this code won't be reached due to infinite loop)
    for (int i = 0; i < 5; i++) {
        close(fds[i]);
    }

    return 0;
}


/*
-----------------------------------------------------------------
output:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/5$ ls
fd_5.c
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/5$ gcc 5.c -o fds
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/5$ ls
fd_5.c  fds
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/5$ ./fds
File f1.txt created with file descriptor: 3
File f2.txt created with file descriptor: 4
File f3.txt created with file descriptor: 5
File f4.txt created with file descriptor: 6
File f5.txt created with file descriptor: 7
-----------------------------------------------------------------
*/


