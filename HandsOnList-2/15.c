#include <stdio.h>
#include <unistd.h>

int main() {
    int fd[2];
    pipe(fd);  

    if (fork() == 0) { 
        char buf[100];
        close(fd[1]);  
        read(fd[0], buf, sizeof(buf));  
        printf("Child received: %s\n", buf);
        close(fd[0]); 
    } 
    else {  
        char wr[] = "Hello parent here!\n";
        close(fd[0]);  
        write(fd[1], wr, sizeof(wr));  
        close(fd[1]); 
    }

    return 0;
}

/*
-------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./15_out
Child received: Hello parent here!

--------------------------------------------------------------------
*/
