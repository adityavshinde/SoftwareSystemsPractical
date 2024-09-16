/*
============================================================================
Name : 8.c
Author : Aditya Vijay Shinde
Description : Write a program to open a file in read only mode, read line by line and display each line as it is read. Close the file when end of file is reached.
Date: 28th Aug, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }


    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error in opening the file");
        return 1;
    }

    char line[256];


    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
    return 0;
}

/*
---------------------------------------------------------------------------------------
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/8$ gcc 8.c -o out8
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/8$ ls
8.c  out8  ToBeReadFile.txt
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/8$ ./out8
Usage: ./out8 <filename>
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/8$ ./out8 ToBeReadFile.txt 
Linux is an open-source operating system based on Unix known for its stability, security, and flexibility. It powers various devices, from personal computers and servers to smartphones. The kernel, the core component, manages hardware resources and facilitates software communication. Users can customize and distribute their own versions, called distributions, like Ubuntu and Fedora. Developers and system administrators favor Linux for its powerful command-line interface and extensive programming support.
----------------------------------------------------------------------------------------
*/
