/*
 * Author- Suyog Pawar
 * Roll No - TIB54
 */


#include "stdio.h"

#define MAXPROCESSESCOUNT 100
#define MAXTOTALEXECUTIONTIME 200


struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
};

struct ganttChartEntry {
    int timeOfPreemption;
    int pid;
};

struct InputProcesses {
    struct Process processes[MAXPROCESSESCOUNT];
    int processCount;
};

struct InputProcesses inputProcesses;
int getProcessCount() { return inputProcesses.processCount; }
void setProcessCount(int count) { inputProcesses.processCount = count; }
int getProcessPid(int index) { return inputProcesses.processes[index].pid; }
void setProcessPid(int index, int pid) { inputProcesses.processes[index].pid = pid; }
int getProcessArrivalTime(int index) { return inputProcesses.processes[index].arrivalTime; }
void setProcessArrivalTime(int index, int arrivalTime) { inputProcesses.processes[index].arrivalTime = arrivalTime; }
int getProcessBurstTime(int index) { return inputProcesses.processes[index].burstTime; }
void setProcessBurstTime(int index, int burstTime) { inputProcesses.processes[index].burstTime = burstTime; }


struct CircularQueue {
    int readyQueue[MAXPROCESSESCOUNT];
    int startIndex, endIndex, readyQueueCount;
};

struct CircularQueue circularQueue;
void incrementStartIndex() { circularQueue.startIndex = (circularQueue.startIndex + 1) % MAXPROCESSESCOUNT; }
void incrementEndIndex() { circularQueue.endIndex = (circularQueue.endIndex + 1) % MAXPROCESSESCOUNT; }
int getStartIndex() { return circularQueue.startIndex; }
int getEndIndex() { return circularQueue.endIndex; }
int getStartIndexEntry() { return circularQueue.readyQueue[circularQueue.startIndex]; }
void setEndIndexEntry(int processIndex) { circularQueue.readyQueue[circularQueue.endIndex] = processIndex; }
int getQueueCount() { return circularQueue.readyQueueCount; }
void setQueueCount(int count) { circularQueue.readyQueueCount = count;}
int getQueueEntry(int index) { return circularQueue.readyQueue[index]; }

void enqueue(int processIndex) {
    incrementEndIndex();
    setEndIndexEntry(processIndex);
    setQueueCount(getQueueCount() + 1);
}

int dequeue() {
    int value = getStartIndexEntry();
    incrementStartIndex();
    setQueueCount(getQueueCount() - 1);
    return value;
}

struct GanttChart {
    struct ganttChartEntry entries[MAXTOTALEXECUTIONTIME];
    int entryCount;
};

struct GanttChart ganttChart;
int getGanttChartEntryCount() { return ganttChart.entryCount; }
void setGanttChartEntryCount(int count) { ganttChart.entryCount = count; }
int getTimeOfPreemption(int entryIndex) { return ganttChart.entries[entryIndex].timeOfPreemption; }
int getEntryPid(int entryIndex) { return ganttChart.entries[entryIndex].pid; }
void setTimeOfPreemption(int currentTime) { ganttChart.entries[ganttChart.entryCount].timeOfPreemption = currentTime; }
void setPid(int pid) { ganttChart.entries[ganttChart.entryCount].pid = pid; }
void incrementEntryCount() { ganttChart.entryCount++; }
void setGanttChartEntry(int currentTime, int pid) {
    setTimeOfPreemption(currentTime);
    setPid(pid);
}

void updateGanttChart(int currentTime, int pid) {
    setGanttChartEntry(currentTime, pid);
    incrementEntryCount();
}

struct SchedulingStats {
    int processExecutedCount;
    int quantumUsed;
    int burstTimesRemaining[MAXPROCESSESCOUNT];
    int completionTimes[MAXPROCESSESCOUNT];
};

struct SchedulingStats stats;
int getProcessExecutedCount() { return stats.processExecutedCount; }
void setProcessExecutedCount(int count) { stats.processExecutedCount = count; }
int getQuantumUsed() { return stats.quantumUsed; }
void setQuantumUsed(int quantum) { stats.quantumUsed = quantum; }
int getRemainingBurstTime(int processIndex) { return stats.burstTimesRemaining[processIndex]; }
void setRemainingBurstTime(int processIndex, int burstTime) {
    stats.burstTimesRemaining[processIndex] = burstTime;
}
int getCompletionTime(int processIndex) { return stats.completionTimes[processIndex]; }
void setCompletionTime(int processIndex, int completionTime) {
    stats.completionTimes[processIndex] = completionTime;
}


void initializeVariables() {
    circularQueue.startIndex = 0;
    circularQueue.endIndex = -1;
    setQueueCount(0);
    setGanttChartEntryCount(0);
    setProcessExecutedCount(0);
    setQuantumUsed(0);
}

void insertArrivingProcesses(int currentTime) {
    int processCount = getProcessCount();
    for (int i = 0; i < processCount; i++) {
        if (getProcessArrivalTime(i) == currentTime)
            enqueue(i);
    }
}

void copyBurstTimes() {
    int processCount = getProcessCount();
    for (int i = 0; i < processCount; i++) {
        setRemainingBurstTime(i, getProcessBurstTime(i));
    }
}

void removeFromQueue(int currentTime, int processIndex) {
    int pid = getProcessPid(processIndex);
    updateGanttChart(currentTime , pid);
    dequeue();
    setCompletionTime(processIndex, currentTime);
    setProcessExecutedCount(getProcessExecutedCount() + 1);
    setQuantumUsed(0);
}

void preemptProcess(int currentTime, int pid) {
    updateGanttChart(currentTime , pid);
    enqueue(dequeue());
    setQuantumUsed(0);
}

void executeProcess(int processIndex) {
    setRemainingBurstTime(processIndex, getRemainingBurstTime(processIndex) - 1);
}

void executeTopProcess() {
    int processIndex = getStartIndexEntry();
    executeProcess(processIndex);
    setQuantumUsed(getQuantumUsed() + 1);
}

int isProcessFinished(int processIndex) {
    return getRemainingBurstTime(processIndex) == 0;
}

void preemptionCheck(int currentTime, int quantum) {

    int processIndex = getStartIndexEntry();
    int pid = getProcessPid(processIndex);

    if (isProcessFinished(processIndex) != 0) {
        removeFromQueue(currentTime, processIndex);
    }
    else if (getQuantumUsed() == quantum) {
        preemptProcess(currentTime, pid);
    }
    else return;
}

void executeRoundRobinCycle(int quantum, int currentTime) {
    executeTopProcess();
    insertArrivingProcesses(currentTime);
    preemptionCheck(currentTime, quantum);
}

void roundRobinScheduling(int quantum) {

    int idle = 0;
    int currentTime = 0;

    insertArrivingProcesses(currentTime);
    if (getQueueCount() == 0) idle = 1;

    while (getProcessExecutedCount() != getProcessCount()) {
        currentTime++;

        if (getQueueCount() > 0 && idle == 1) {
            idle = 0;
            updateGanttChart(currentTime - 1, -1);
            executeRoundRobinCycle(quantum, currentTime);

        } else if (getQueueCount() == 0 && idle == 0) {
            idle = 1;
            insertArrivingProcesses(currentTime);
        }
        else if (getQueueCount() > 0 && idle == 0) {
            executeRoundRobinCycle(quantum, currentTime);
        }
        else {
            insertArrivingProcesses(currentTime);
        }
    }
}

int findShortestJob() {

    int minJobIndex = getStartIndexEntry();

    for (int i = getStartIndex(); i <= getEndIndex(); i++)  {
        int processIndex = getQueueEntry(i);
        if (getRemainingBurstTime(processIndex) < getRemainingBurstTime(minJobIndex)) {
            minJobIndex = processIndex;
        }
    }

    return minJobIndex;
}

// Problems here and below.
int executeShortJobCycle(int currentTime) {
    int processIndex = findShortestJob();
    executeProcess(processIndex);

    if (isProcessFinished(processIndex) != 0) {
        removeFromQueue(currentTime, processIndex);
        return -1;
    }
    else {
        return processIndex;
    }
}

void shortestJobFirstScheduling() {

    int idle = 0;
    int currentTime = 0;
    int processExecutionCompleted = 0;

    insertArrivingProcesses(currentTime);

    while (getProcessExecutedCount() != getProcessCount()) {
        if (getQueueCount() == 0 && idle == 0) {
            idle = 1;
        }
        else if (getQueueCount() > 0 && idle == 1) {

        }
    }
}


void printGanttChart() {
    printf("\nGantt Chart\n");
    int previousTime = 0;
    for (int i = 0; i < getGanttChartEntryCount(); i++) {
        printf("From %i to %i -> pid %i Executed\n",
               previousTime, getTimeOfPreemption(i), getEntryPid(i));
        previousTime = getTimeOfPreemption(i);
    }
}

void printStats() {
    float totalTurnAroundTime = 0, totalWaitingTime = 0;
    for (int i = 0; i < inputProcesses.processCount; i++) {
        totalTurnAroundTime += (stats.completionTimes[i] - inputProcesses.processes[i].arrivalTime);
        totalWaitingTime += (stats.completionTimes[i] - inputProcesses.processes[i].arrivalTime
                            - inputProcesses.processes[i].burstTime);
    }
    printf("\nAverage Turn Around Time: %.3f", totalTurnAroundTime / inputProcesses.processCount);
    printf("\nAverage Waiting Time: %.3f\n", totalWaitingTime / inputProcesses.processCount);
}

void input() {
    printf("Enter number of Processes: ");
    scanf("%i", &inputProcesses.processCount);
    for (int i = 0; i < inputProcesses.processCount; i++) {
        printf("\nEnter pid of %i Process: ", i);
        scanf("%i", &inputProcesses.processes[i].pid);
        printf("Enter arrival time of %i Process: ", i);
        scanf("%i", &inputProcesses.processes[i].arrivalTime);
        printf("Enter burst time of %i Process: ", i);
        scanf("%i", &inputProcesses.processes[i].burstTime);
    }

}

int inputQuantum() {
    int quantum;
    printf("Enter time quantum: ");
    scanf("%i", &quantum);
    return quantum;
}

int main() {
    int choice;
    do {
        printf("\nMenu\n1. Round Robin Scheduling.\n2. Shortest Job First.\n3. Exit.\n");
        printf("Enter your choice: ");
        scanf("%i", &choice);

        switch (choice) {
            case 1:
                initializeVariables();
                input();
                copyBurstTimes();
                roundRobinScheduling(inputQuantum());
                printGanttChart();
                printStats();
                break;
            case 2:
                initializeVariables();
                input();
                copyBurstTimes();
                shortestJobFirstScheduling();
                printGanttChart();
                printStats();
                break;
            default:
                break;
        }
    } while (choice != 3);
    return 0;
}

