/*
============================================================================
Name : 1b.c
Author : Aditya Vijay Shinde
Description : Create b.hard link file using system call
Date: 21st Aug, 2024.
============================================================================
*/

#include <unistd.h>
#include <stdio.h>

int main() {
    const char *original = "/home/g3n1u5/SS/SoftwareSystemsPractical/Hardlink/myfile.txt";
    const char *copy = "/home/g3n1u5/shortcut_hl.txt";
    if (link(original, copy) == -1) {
        perror("link failed");
    } else {
        printf("Hard link has been created.\n");
    }
    return 0;
}

/*
----------------------------------------------------------------
output:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/Hardlink$ ./outfile
Hard link has been created.
----------------------------------------------------------------
*/
