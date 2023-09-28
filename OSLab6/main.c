#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const int NEVERUSED = 100;

void printFrames(int* frames, int frameSize) {
    for (int i = 0; i < frameSize; i++) {
        printf("%i ", frames[i]);
    }
}

bool isPresentInFrames(int pageReference, const int* frames, int frameSize) {
    for (int i = 0; i < frameSize; i++) {
        if (frames[i] == pageReference) {
            return true;
        }
    }
    return false;
}

void enqueue(int pageReference, int* frames, int frameSize, int* backOfQueue) {
    frames[*backOfQueue] = pageReference;
    *backOfQueue = ((*backOfQueue) + 1) % frameSize;
}

void initializeArrayWithValue(int* array, int arraySize, int value) {
    for (int i = 0; i < arraySize; i++) {
        array[i] = value;
    }
}

void printIterationResult(int frameSize, const int *pageReferences, int *frames, bool hit, int pageReferenceIndex) {
    printf("\n%i: ", pageReferences[pageReferenceIndex]);
    printFrames(frames, frameSize);
    if (hit == false) {
        printf("Miss");
    } else printf("Hit");
}

void FCFS(int frameSize, int* pageReferences, int pageReferencesCount) {

    int* frames = malloc(sizeof(int) * frameSize);
    initializeArrayWithValue(frames, frameSize, -1);

    int backOfQueue = 0, pageFaults = 0;

    for (int i = 0; i < pageReferencesCount; i++) {

        bool hit = isPresentInFrames(pageReferences[i], frames, frameSize);

        if (hit == false) {
            pageFaults++;
            enqueue(pageReferences[i], frames, frameSize, &backOfQueue);
        }

        printIterationResult(frameSize, pageReferences, frames, hit, i);
    }

    printf("\nPage faults: %i\n", pageFaults);
    free(frames);
}


int getLeastUsedPageIndex(const int* usages, int usagesSize) {

     int leastUsedPageTime = 0, leastUsedPageIndex = -1;

    for (int i = 0; i < usagesSize; i++) {
        if (usages[i] > leastUsedPageTime) {
            leastUsedPageTime = usages[i];
            leastUsedPageIndex = i;
        }
    }
    return leastUsedPageIndex;
}

void resetUsage(int pageReference, const int* frames, int framesSize, int* usages) {
    int index = 0;

    for (int i = 0; i < framesSize; i++) {
        if (frames[i] == pageReference) {
            index = i;
            break;
        }
    }
    usages[index] = 0;
}

void updateUsages(int* usages, int usagesSize) {
    for (int i = 0; i < usagesSize; i++) {
        if (usages[i] != NEVERUSED) {
            usages[i]++;
        }
    }
}

void LRU(int frameSize, int* pageReferences, int pageReferencesCount) {

    int* frames = malloc(sizeof(int) * frameSize);
    initializeArrayWithValue(frames, frameSize, -1);

    int usagesSize = frameSize;
    int* usages = malloc(sizeof(int) * usagesSize);
    initializeArrayWithValue(usages, usagesSize, NEVERUSED);

    int pageFaults = 0;

    for (int i = 0; i < pageReferencesCount; i++) {

        bool hit = isPresentInFrames(pageReferences[i], frames, frameSize);

        if (hit == false) {
            int leastUsedPageIndex = getLeastUsedPageIndex(usages, usagesSize);
            frames[leastUsedPageIndex] = pageReferences[i];
            resetUsage(frames[leastUsedPageIndex], frames, frameSize, usages);
            pageFaults++;
        } else {
            resetUsage(pageReferences[i], frames, frameSize, usages);
        }

        updateUsages(usages, usagesSize);
        printIterationResult(frameSize, pageReferences, frames, hit, i);
    }

    printf("\nPage faults: %i\n", pageFaults);

    free(frames);
    free(usages);
}

int getOptimalReplacementIndex(int startingIndex, const int *pageReferences, int pageReferencesCount,
                                   const int *frames, int frameSize) {

    int optimalPageIndex = 0, longestTimeOfFutureUse = 0;

    for (int i = 0; i < frameSize; i++) {

        int pageReference = frames[i], timeOfFutureUse = NEVERUSED;

        if (pageReference == -1) {
            return i;
        }

        for (int j = startingIndex; j < pageReferencesCount; j++) {
            if (pageReferences[j] == pageReference) {
                timeOfFutureUse = j;
                break;
            }
        }
        if (timeOfFutureUse > longestTimeOfFutureUse) {
            longestTimeOfFutureUse = timeOfFutureUse;
            optimalPageIndex = i;
        }

    }
    return optimalPageIndex;
}

void Optimal(int frameSize, int* pageReferences, int pageReferencesCount) {

    int* frames = malloc(sizeof(int) * frameSize);
    initializeArrayWithValue(frames, frameSize, -1);

    int pageFaults = 0;

    for (int i = 0; i < pageReferencesCount; i++) {

        bool hit = isPresentInFrames(pageReferences[i] ,frames, frameSize);

        if (hit == false) {
            int optimalIndex = getOptimalReplacementIndex(i + 1, pageReferences, pageReferencesCount,
                                                          frames, frameSize);
            frames[optimalIndex] = pageReferences[i];
            pageFaults++;
        }

        printIterationResult(frameSize, pageReferences, frames, hit, i);
    }

    printf("\nPage faults: %i\n", pageFaults);

    free(frames);
}


int inputFrameSize() {
    int frameSize;
    printf("Frame size: ");
    scanf("%i", &frameSize);
    return frameSize;
}

int inputPageReferencesCount() {
    int pageReferencesCount;
    printf("Number of page references: ");
    scanf("%i", &pageReferencesCount);
    return pageReferencesCount;
}

void inputPageReferences(int pageReferencesCount, const int *pageReferences) {
    for (int i = 0; i < pageReferencesCount; i++) {
        printf("Enter page reference: ");
        scanf("%d", &pageReferences[i]);
    }
}

int main() {

    int frameSize, pageReferencesCount, choice;
    int* pageReferences = NULL;

    do {
        printf("\nMenu\n1. FCFS\n2. LRU\n3. Optimal\n4. Exit\nEnter your choice: ");
        scanf("%i", &choice);

        switch (choice) {
            case 1:
                frameSize = inputFrameSize();
                pageReferencesCount = inputPageReferencesCount();
                pageReferences = malloc(sizeof(int) * pageReferencesCount);
                inputPageReferences(pageReferencesCount, pageReferences);
                FCFS(frameSize, pageReferences, pageReferencesCount);
                free(pageReferences);
                break;
            case 2:
                frameSize = inputFrameSize();
                pageReferencesCount = inputPageReferencesCount();
                pageReferences = malloc(sizeof(int) * pageReferencesCount);
                inputPageReferences(pageReferencesCount, pageReferences);
                LRU(frameSize, pageReferences, pageReferencesCount);
                free(pageReferences);
                break;
            case 3:
                frameSize = inputFrameSize();
                pageReferencesCount = inputPageReferencesCount();
                pageReferences = malloc(sizeof(int) * pageReferencesCount);
                inputPageReferences(pageReferencesCount, pageReferences);
                Optimal(frameSize, pageReferences, pageReferencesCount);
                free(pageReferences);
                break;
            default:
                break;
        }
    } while (choice != 4);

    return 0;
}
