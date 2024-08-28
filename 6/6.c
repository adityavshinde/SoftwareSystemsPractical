#include<stdio.h>
#include<unistd.h>
 int main( int argc, char *argv[]) {
	 char buffer[32];
	 int sie_read = read(0, buffer, 20);
	 printf("%d", sie_read);
	 fflush(stdout);
	 write(1, buffer, sie_read);
	 return 0;
 }

