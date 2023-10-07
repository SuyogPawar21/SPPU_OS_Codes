#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define INFINITE 999999;

void initializeFinishedArray(bool isRequestFinished[], int requestCount) {
    for (int i = 0; i < requestCount; i++) {
        isRequestFinished[i] = false;
    }
}

int getShortestSeekIndex(int requests[], int requestCount, const bool isRequestFinished[], int currentPosition) {

    int shortestSeekRequestIndex = -1;
    int shortestSeekDistance = INFINITE;

    for (int i = 0; i < requestCount; i++) {
        if (isRequestFinished[i] == false && shortestSeekDistance > abs(currentPosition - requests[i])) {
            shortestSeekDistance = abs(currentPosition - requests[i]);
            shortestSeekRequestIndex = i;
        }
    }
    return shortestSeekRequestIndex;
}

void printRequestFulfillment(int requestIndex, const int *requests, int currentPosition, int overheadMovement) {
    printf("%i -> %i: %i\n", currentPosition, requests[requestIndex], overheadMovement);
}

int fulfillRequest(int requestIndex , int *requests, bool *isRequestFinished, int* currentPosition) {

    int overheadMovement = abs(*currentPosition - requests[requestIndex]);
    printRequestFulfillment(requestIndex, requests, *currentPosition, overheadMovement);
    *currentPosition = requests[requestIndex];
    isRequestFinished[requestIndex] = true;
    return overheadMovement;
}

void SSTF(int requests[], int requestCount, int startPosition, int endPosition) {

    int currentPosition = startPosition;
    int requestsRemaining = requestCount;
    int totalOverheadMovement = 0;

    bool isRequestFinished[requestCount];
    initializeFinishedArray(isRequestFinished, requestCount);

    while (requestsRemaining > 0) {

        int shortestSeekRequestIndex = getShortestSeekIndex(requests, requestCount,
                                                                   isRequestFinished, currentPosition);

        totalOverheadMovement += fulfillRequest(shortestSeekRequestIndex, requests,
                                                isRequestFinished, &currentPosition);
        requestsRemaining--;
    }

    printf("Total Overhead Movement: %i\n", totalOverheadMovement);
}

int getNextLargerRequestIndex(const int requests[], int requestCount,
                              int currentPosition, const bool isRequestFinished[]) {

    int nextLargerRequestIndex = -1;
    int nextLargerRequest = INFINITE;

    for (int requestIndex = 0; requestIndex < requestCount; requestIndex++) {

        if (isRequestFinished[requestIndex] == false &&
        requests[requestIndex] >= currentPosition &&
        requests[requestIndex] < nextLargerRequest) {
            nextLargerRequestIndex = requestIndex;
            nextLargerRequest = requests[requestIndex];
        }

    }

    return nextLargerRequestIndex;
}

int getNextLowerRequestIndex(const int requests[], int requestCount,
                              int currentPosition, const bool isRequestFinished[]) {

    int nextLowerRequestIndex = -1;
    int nextLowerRequest = 0;

    for (int requestIndex = 0; requestIndex < requestCount; requestIndex++) {

        if (isRequestFinished[requestIndex] == false &&
            requests[requestIndex] <= currentPosition &&
            requests[requestIndex] > nextLowerRequest) {
            nextLowerRequestIndex = requestIndex;
            nextLowerRequest = requests[requestIndex];
        }

    }

    return nextLowerRequestIndex;
}

void SCAN(int requests[], int requestCount, int startPosition, int endPosition) {

    int totalOverheadMovement = 0;
    int currentPosition = startPosition;

    bool isRequestFinished[requestCount];
    initializeFinishedArray(isRequestFinished, requestCount);

    while (currentPosition < endPosition) {

        int nextLargeRequestIndex = getNextLargerRequestIndex(requests, requestCount,
                                                              currentPosition, isRequestFinished);

        if (nextLargeRequestIndex == -1) break;

        totalOverheadMovement += fulfillRequest(nextLargeRequestIndex, requests,
                                                isRequestFinished, &currentPosition);

    }

    if (currentPosition < endPosition) {
        totalOverheadMovement += endPosition - currentPosition;
        currentPosition = endPosition;
    }

    while (currentPosition > 0) {

        int nextLowerRequestIndex = getNextLowerRequestIndex(requests, requestCount,
                                                              currentPosition, isRequestFinished);

        if (nextLowerRequestIndex == -1) break;

        totalOverheadMovement += fulfillRequest(nextLowerRequestIndex, requests,
                                                isRequestFinished, &currentPosition);

    }

    printf("Total Overhead Movement: %i\n", totalOverheadMovement);
}

void CLook(int requests[], int requestCount, int startPosition, int endPosition) {

    int totalOverheadMovement = 0;
    int currentPosition = startPosition;

    bool isRequestFinished[requestCount];
    initializeFinishedArray(isRequestFinished, requestCount);

    bool endReached = false;

    while (currentPosition < endPosition) {

        int nextLargeRequestIndex = getNextLargerRequestIndex(requests, requestCount,
                                                              currentPosition, isRequestFinished);

        if (nextLargeRequestIndex == -1) {
            if (endReached == false) {
                endReached = true;
                nextLargeRequestIndex = getNextLargerRequestIndex(requests, requestCount,
                                                                      0, isRequestFinished);
                printf("Reset: %i -> %i: %i\n", currentPosition, requests[nextLargeRequestIndex],
                       currentPosition - requests[nextLargeRequestIndex]);
                totalOverheadMovement += currentPosition - requests[nextLargeRequestIndex];
                currentPosition = requests[nextLargeRequestIndex];
                continue;
            } else break;
        }

        totalOverheadMovement += fulfillRequest(nextLargeRequestIndex, requests,
                                                isRequestFinished, &currentPosition);

    }

    printf("Total Overhead Movement: %i\n", totalOverheadMovement);
}

int* input(int* requestCount, int* startPosition, int* endPosition) {

    printf("Enter request count: ");
    scanf("%i", requestCount);

    int* requests = malloc(sizeof(int) * (*requestCount));

    for (int i = 0; i < (*requestCount); i++) {
        printf("Enter %i request: ", i);
        scanf("%i", &requests[i]);
    }

    printf("Enter start position: ");
    scanf("%i", startPosition);

    printf("Enter end position: ");
    scanf("%i", endPosition);

    return requests;
}

int main() {

    int requestCount, choice = -1;
    int startPosition, endPosition;
    int* requests;

    do {
        printf("\nMenu\n1. SSTF\n2. SCAN\n3. CLook\n4. Exit\nEnter your choice: ");
        scanf("%i", &choice);

        switch (choice) {
            case 1:
                requests = input(&requestCount, &startPosition, &endPosition);
                SSTF(requests, requestCount, startPosition, endPosition);
                free(requests);
                break;
            case 2:
                requests = input(&requestCount, &startPosition, &endPosition);
                SCAN(requests, requestCount, startPosition, endPosition);
                free(requests);
                break;
            case 3:
                requests = input(&requestCount, &startPosition, &endPosition);
                CLook(requests, requestCount, startPosition, endPosition);
                free(requests);
                break;
            default:
                break;
        }
    } while (choice != 4);

    return 0;
}