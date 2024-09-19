#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int pipefd[2];
    pipe(pipefd);  

    if (fork() == 0) {  
        close(pipefd[0]);  
        dup2(pipefd[1], STDOUT_FILENO);  
        close(pipefd[1]);
        execlp("ls", "ls", "-l", NULL);  
        exit(1);
    } else {  
        close(pipefd[1]);  
        dup2(pipefd[0], STDIN_FILENO);  
        close(pipefd[0]);
        execlp("wc", "wc", NULL);  
        exit(1);
    }

    return 0;
}

/*
---------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./17b_out
     72     641    3698
---------------------------------------------------------------------
*/
