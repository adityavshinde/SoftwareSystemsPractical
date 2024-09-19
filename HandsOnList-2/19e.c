#include <sys/types.h>
#include <sys/stat.h>

int main() {
    mkfifo("fifo_mkfifo", 0666);  
    return 0;
}

/*
-------------------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./19e_out
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ls -l fifo_mkfifo
prw-rw-r-- 1 g3n1u5 g3n1u5 0 Sep 20 02:18 fifo_mkfifo
-------------------------------------------------------------------------------
*/



