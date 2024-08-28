#include <unistd.h>

int main() {
    if (fork() == 0) { // Child process
        while (1) {
            sleep(5); // Infinite loop with 5 seconds sleep
        }
    }
    return 0; // Parent process exits
}
