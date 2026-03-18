#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(
    const int argc, char *argv[]
) {
    if (argc < 2) {
        printf("usage: %s dir\n", argv[0]);
    }
    DIR *d = opendir(".");
    if (d) {
        struct dirent *dir;
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
        }
    } else {
        printf("opendir error\n");
    }
}
