#include <stdio.h>

void printArrayReverse(char** strings, int stringCount) {
    for (int i = stringCount - 1; i >= 0; i--) {
        printf("%s ", strings[i]);
    }
    printf("\n");
}

int main(int argc , char** argv) {
    printf("Child Process: ");
    printArrayReverse(argv, argc);
    return 0;
}
