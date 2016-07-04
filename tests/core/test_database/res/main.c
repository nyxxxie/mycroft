#include <stdio.h>

char ye[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
int print_hello_world() {
    for (int i = 0; i < sizeof(ye); i++) {
        printf("%c", ye[i]);
    }
}

int main(int argc, char *argv[]) {
    print_hello_world();
    butt_actually();
    return 0;
}
