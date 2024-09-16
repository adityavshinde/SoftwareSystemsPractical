/*
============================================================================
Name : 9.c
Author : Aditya Vijay Shinde
Description : Write a program to print the following information about a given file.
a. inode
b. number of hard links
c. uid
d. gid
e. size
f. block size
g. number of blocks
h. time of last access
i. time of last modification
j. time of last change
Date: 29th Aug, 2024.
============================================================================
*/
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    struct stat x;


    if (stat(argv[1], &x) == -1) {
        perror("Error in getting the file stats");
        return 1;
    }

    printf("File: %s\n", argv[1]);
    printf("A.Inode number: %ld\n", x.st_ino);
    printf("B.Number of hard links: %ld\n", x.st_nlink);
    printf("C.User ID (UID): %d\n", x.st_uid);
    printf("D.Group ID (GID): %d\n", x.st_gid);
    printf("E.File size: %ld bytes\n", x.st_size);
    printf("F.Block size: %ld bytes\n", x.st_blksize);
    printf("G.Number of blocks: %ld\n", x.st_blocks);
    printf("H.Time of last access: %s", ctime(&x.st_atime));
    printf("I.Time of last modification: %s", ctime(&x.st_mtime));
    printf("J.Time of last status change: %s", ctime(&x.st_ctime));

    return 0;
}

/*
----------------------------------------------------------------
output:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/9$ ./of9 file.txt
File: file.txt
Inode number: 6320372
Number of hard links: 1
User ID (UID): 1000
Group ID (GID): 1000
File size: 24 bytes
Block size: 4096 bytes
Number of blocks: 8
Time of last access: Thu Aug 29 14:52:59 2024
Time of last modification: Thu Aug 29 14:53:17 2024
Time of last status change: Thu Aug 29 14:53:17 2024
---------------------------------------------------------------
*/
