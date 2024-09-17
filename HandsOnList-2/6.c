#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* thread_function(void* arg) {
    printf("Thread %lu is running\n", pthread_self());  //pthread_self() takes thread id itself
    return NULL;
}

int main() {
    pthread_t threads[3];           // Array to hold thread IDs            
    int i;
    for (i = 0; i < 3; i++) {  
        if (pthread_create(&threads[i], NULL, thread_function, NULL) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads have finished.\n");
    return 0;
}

/*
--------------------------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ gcc -o 6_out 6.c -pthread
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./6_out
Thread 140431123801792 is running
Thread 140431115409088 is running
Thread 140431132194496 is running
All threads have finished.
--------------------------------------------------------------------------------------
*/

