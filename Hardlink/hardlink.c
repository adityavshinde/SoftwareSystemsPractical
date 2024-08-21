#include <unistd.h>
#include <stdio.h>

int main() {
    const char *original = "/home/g3n1u5/SS/SoftwareSystemsPractical/Hardlink/myfile.txt";
    const char *copy = "/home/g3n1u5/shortcut_hl.txt";
    if (link(original, copy) == -1) {
        perror("link failed");
    } else {
        printf("Hard link has been created.\n");
    }
    return 0;
}
