#include <stdio.h>
#include <stdbool.h>

#define MAXPROCESSES 10

struct Resources {
    int A;
    int B;
    int C;
};

int processesCount = 5;
struct Resources allocations[MAXPROCESSES];
struct Resources max[MAXPROCESSES];
struct Resources available;
struct Resources need[MAXPROCESSES];
int executionSequence[MAXPROCESSES];

void calculateNeed() {
    for (int i = 0 ; i < processesCount; i++) {
        need[i].A = max[i].A - allocations[i].A;
        need[i].B = max[i].B - allocations[i].B;
        need[i].C = max[i].C - allocations[i].C;
    }
}

bool canBeFulfilled(int processIndex) {
    return (need[processIndex].A <= available.A) &&
           (need[processIndex].B <= available.B) && (need[processIndex].C <= available.C);
}

void fulfillRequest(int processIndex) {
    available.A += allocations[processIndex].A;
    available.B += allocations[processIndex].B;
    available.C += allocations[processIndex].C;
}

void printAvailableResources() {
    printf("Available Resources: A-%i, B-%i, C-%i\n", available.A, available.B, available.C);
}

void BankersAlgo() {

    calculateNeed();

    int processesRemaining[MAXPROCESSES];
    int processesRemainingCount = processesCount;

    for (int i = 0; i < processesCount; i++) {
        processesRemaining[i] = 1;
    }

    while (processesRemainingCount > 0) {
        for (int i = 0; i < processesCount; i++) {
            if (processesRemaining[i] == 1) {
                if (canBeFulfilled(i) == true) {
                    fulfillRequest(i);
                    printf("\n%ith Process Executed\n", i);
                    printAvailableResources();
                    executionSequence[processesCount - processesRemainingCount] = i;
                    processesRemainingCount--;
                    processesRemaining[i] = 0;
                }
            }
        }
    }

}

void printExecutionSequence() {
    printf("\nStart->");
    for (int i = 0; i < processesCount; i++) {
        printf("P%i->", executionSequence[i]);
    }
    printf("End");
}

void input() {
    printf("Enter number of Processes: ");
    scanf("%i", &processesCount);

    for (int i = 0; i < processesCount; i++) {

        printf("Enter number of allocations of resource of type A for %i Process: ", i);
        scanf("%i", &allocations[i].A);

        printf("Enter number of allocations of resource of type B for %i Process: ", i);
        scanf("%i", &allocations[i].B);

        printf("Enter number of allocations of resource of type C for %i Process: ", i);
        scanf("%i", &allocations[i].C);

    }

    for (int i = 0; i < processesCount; i++) {

        printf("Enter number of resources required of type A for %i Process: ", i);
        scanf("%i", &max[i].A);

        printf("Enter number of resources required of type B for %i Process: ", i);
        scanf("%i", &max[i].B);

        printf("Enter number of resources required of type C for %i Process: ", i);
        scanf("%i", &max[i].C);

    }

    printf("Enter number of resources available of type A: ");
    scanf("%i", &available.A);

    printf("Enter number of resources available of type B: ");
    scanf("%i", &available.B);

    printf("Enter number of resources available of type C: ");
    scanf("%i", &available.C);

}

int main() {
    input();
    BankersAlgo();
    printExecutionSequence();
    return 0;
}