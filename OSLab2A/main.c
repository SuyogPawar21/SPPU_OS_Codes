#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

void bubbleSortAscent(int* nums, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (nums[j] > nums[j + 1]) {
                int temp = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = temp;
            }
        }
    }
}

void bubbleSortDescent(int* nums, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (nums[j] < nums[j + 1]) {
                int temp = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = temp;
            }
        }
    }
}

void printArray(int* nums, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");
}

int main() {

    int n, temp;
    int nums[100];
    printf("Enter total number of elements: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("Enter %d element: ", i);
        scanf("%d", &temp);
        nums[i] = temp;
    }

    printf("\n");

    int childPid = fork();

    if (childPid == 0) {
        printf("Child Process Output: ");
        bubbleSortDescent(nums, n);
        printArray(nums, n);
    }
    else {
        wait(NULL);
        printf("Parent Process Output: ");
        bubbleSortAscent(nums, n);
        printArray(nums, n);
    }
    return 0;
}
