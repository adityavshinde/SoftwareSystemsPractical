#include <unistd.h>
#include <stdio.h>

int main() {
    const char *original = "/home/g3n1u5/SS/SoftwareSystemsPractical/Softlink/myfile.txt";
    const char *copy = "/home/g3n1u5/shortcut.txt";
    if (symlink(original, copy) == -1) {
        perror("soft link failed");
    } else {
        printf("Soft link has been created.\n");
    }
    return 0;
}

//output:
//g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/Softlink$ ./outfile
//Soft link has been created.
