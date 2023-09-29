#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <malloc/_malloc.h>

const int MAXSTRINGCOUNT = 10;

void bubbleSort(char** strings, int stringCount) {

    for (int i = 0; i < stringCount; i++) {
        for (int j = 0; j < stringCount - i - 1; j++) {
            if (strcmp(strings[j], strings[j + 1]) > 0) {
                char* temp = strings[j];
                strings[j] = strings[j + 1];
                strings[j + 1] = temp;
            }
        }
    }

}

void printStrings(char** strings, int stringCount) {
    for (int i = 0; i < stringCount; i++) {
        printf("%s ", strings[i]);
    }
}

void freeMemory(char** strings, int stringCount) {
    for (int i = 0; i < stringCount; i++) {
        free(strings[i]);
    }
}

int main() {

    int stringCount, stringSize;
    printf("Enter the number of strings: ");
    scanf("%i", &stringCount);

    char** strings[MAXSTRINGCOUNT];

    for (int i = 0; i < stringCount; i++) {

        printf("Enter string size: ");
        scanf("%i", &stringSize);

        strings[i] = malloc(stringSize + 1);

        printf("Enter string: ");
        scanf("%s", strings[i]);

    }

    bubbleSort(strings, stringCount);

    pid_t childPid = fork();

    if (childPid == 0) {
        execve("OSLab2B/child.exe", strings, NULL);
    } else {
        wait(NULL);
        printf("Parent Process: ");
        printStrings(strings, stringCount);
        freeMemory(strings, stringCount);
    }

    return 0;
}
