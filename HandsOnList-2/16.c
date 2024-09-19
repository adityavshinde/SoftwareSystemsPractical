#include <stdio.h>
#include <unistd.h>

int main() {
    int p2c[2], c2p[2];  
    pipe(p2c);  
    pipe(c2p);  

    if (fork() == 0) {  
        char buf[100];
        read(p2c[0], buf, sizeof(buf));  
        printf("Child received: %s\n", buf);
        write(c2p[1], "Hello, Parent, Child Here", 26);  
    } else {  
        write(p2c[1], "Hello, Child, Parent Here", 26); 
        char buf[100];
        read(c2p[0], buf, sizeof(buf));  
        printf("Parent received: %s\n", buf);
    }

    return 0;
}

/*
---------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./16_out
Child received: Hello, Child, Parent Here
Parent received: Hello, Parent, Child Here
---------------------------------------------------------
*/

