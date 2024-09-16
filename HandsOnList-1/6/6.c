/*
============================================================================
Name : 6.c
Author : Aditya Vijay Shinde
Description : Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls
Date: 28th Aug, 2024.
============================================================================
*/
#include<stdio.h>
#include<unistd.h>
 int main( int argc, char *argv[]) {
	 char buffer[32];
	 int sie_read = read(0, buffer, 20);
	 printf("%d\n", sie_read);
	 fflush(stdout);
	 write(1, buffer, sie_read);
	 return 0;
 }
/*
-----------------------------------------------------
output:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/6$ ./of6
hello world
12
hello world
-----------------------------------------------------
*/
