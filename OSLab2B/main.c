#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXNUMBERSCOUNT 100

void bubbleSort(int numbers[], int numbersCount) {
    for (int i = 0; i < numbersCount; i++) {
        for (int j = 0; j < numbersCount - i - 1; j++) {
            if (numbers[j] > numbers[j + 1]) {
                int temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }
}

int main() {

    int numbers[MAXNUMBERSCOUNT];
    int numbersCount;

    printf("Enter the number of numbers: ");
    scanf("%i", &numbersCount);

    for (int i = 0; i < numbersCount; i++) {
        printf("Enter number: ");
        scanf("%i", &numbers[i]);
    }

    bubbleSort(numbers, numbersCount);

    pid_t childPid = fork();

    if (childPid == 0) {
        execv();
    } else {
        printf("Child's Process ID: %i", childPid);
        wait(NULL);
    }

    return 0;
}
