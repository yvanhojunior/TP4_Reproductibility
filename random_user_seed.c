#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <seed>\n", argv[0]);
        return 1;
    }

    int seed = atoi(argv[1]);
    srand(seed);

    for (int i = 0; i < 5; i++) {
        printf("%d\n", rand());
    }
    return 0;
}