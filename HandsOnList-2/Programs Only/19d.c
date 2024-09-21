/*
============================================================================
Name : 19d.c
Author : Aditya Shinde
Description : Create a FIFO file by mknod system call
Date: 19th Sep, 2024.
============================================================================
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    mknod("fifo_mknod", S_IFIFO | 0666, 0); 
    return 0;
}

/*
-------------------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./19d_out
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ls -l fifo_mknod
prw-rw-r-- 1 g3n1u5 g3n1u5 0 Sep 20 02:17 fifo_mknod
-------------------------------------------------------------------------------
*/
