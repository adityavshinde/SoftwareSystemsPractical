#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd[2];
    pipe(fd); 

    if (fork() == 0) {  
        close(fd[0]);
        close(STDOUT_FILENO);
        fcntl(fd[1], F_DUPFD, 1);  
        execlp("ls", "ls", "-l", NULL);
    } else {  
        close(fd[1]);
        close(STDIN_FILENO);
        fcntl(fd[0], F_DUPFD, 0);  
        execlp("wc", "wc", NULL);
    }

    return 0;
}


/*
---------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./17c_out
     72     641    3698
---------------------------------------------------------------------
*/
