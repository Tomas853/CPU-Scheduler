#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Headers as needed
#define TIME_QUANTUM 2


typedef enum { false, true } bool;        // Allows boolean types in C

/* Defines a job struct */
typedef struct Process {
    uint32_t A;                         // A: Arrival time of the process
    uint32_t B;                         // B: Upper Bound of CPU burst times of the given random integer list
    uint32_t C;                         // C: Total CPU time required
    uint32_t M;                         // M: Multiplier of CPU burst time
    uint32_t processID;                 // The process ID given upon input read
    uint32_t randOffset;

    uint32_t remainingCPUTime;
    uint32_t currentCPUTimeRun;
    uint32_t currentIOBlockedTime;
    uint32_t currentWaitingTime;
    uint32_t finishingTime;
    uint32_t currentCPUBurst;
    uint32_t timeUntilUnblock;
    uint32_t remainingIOTime;

    bool isBlocked;
    bool isReady;
    bool isRunning;
    bool isFinished;
} _process;

uint32_t TOTAL_CREATED_PROCESSES = 0;   // The total number of processes constructed
uint32_t TOTAL_FINISHED_PROCESSES = 0;
uint32_t CURRENT_CYCLE = 0;
uint32_t TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED = 0;

// ======================Queue stuff======================
typedef struct node {
    _process* data;
    struct node* next;
} Node;

typedef struct queue {
    struct node* head;
    struct node* tail;
} Queue;


void initQueue(Queue* q) {
    q->head = q->tail = NULL;
}

bool isEmpty(Queue* q) {
    return q->head == NULL;
}

void enqueue(Queue* q, _process* process) {
    Node* newNode = malloc(sizeof(Node));
    newNode->data = process;
    newNode->next = NULL;

    if (isEmpty(q)) {
        q->head = q->tail = newNode;
    }
    else {
        q->tail->next = newNode;
        q->tail = newNode;
    }
}

_process* dequeue(Queue* q) {
    if (isEmpty(q)) return NULL;
    Node* tmp = q->head;
    _process* process_old = tmp->data;
    q->head = q->head->next;
    free(tmp);
    return process_old;
}
//=============Queue stuff end =====================


/**
 * Reads a random non-negative integer X from a file with a given line named random-numbers (in the current directory)
 */
uint32_t getRandNumFromFile(uint32_t line, FILE* random_num_file_ptr) {
    uint32_t end, loop;
    char str[512];

    rewind(random_num_file_ptr); // reset to be beginning
    for (end = loop = 0;loop < line;++loop) {
        if (0 == fgets(str, sizeof(str), random_num_file_ptr)) { //include '\n'
            end = 1;  //can't input (EOF)
            break;
        }
    }
    if (!end) {
        return (uint32_t)atoi(str);
    }

    // Fail-safe return
    return (uint32_t)1804289383;
}



/**
 * Reads a random non-negative integer X from a file named random-numbers.
 * Returns the CPU Burst: : 1 + (random-number-from-file % upper_bound)
 */
uint32_t randomOS(uint32_t upper_bound, uint32_t offset,FILE* random_num_file_ptr)
{
    char str[20];

    uint32_t unsigned_rand_int = (uint32_t)getRandNumFromFile(offset, random_num_file_ptr);
    uint32_t returnValue = 1 + (unsigned_rand_int % upper_bound);

    return returnValue;
}


/********************* SOME PRINTING HELPERS *********************/


/**
 * Prints to standard output the original input
 * process_list is the original processes inputted (in array form)
 */
void printStart(_process process_list[])
{
    printf("The original input was: %i", TOTAL_CREATED_PROCESSES);

    uint32_t i = 0;
    for (; i < TOTAL_CREATED_PROCESSES; ++i)
    {
        printf(" ( %i %i %i %i)", process_list[i].A, process_list[i].B,
            process_list[i].C, process_list[i].M);
    }
    printf("\n");
}

/**
 * Prints to standard output the final output
 * finished_process_list is the terminated processes (in array form) in the order they each finished in.
 */
void printFinal(_process finished_process_list[])
{
    printf("The (sorted) input is: %i", TOTAL_CREATED_PROCESSES);

    uint32_t i = 0;
    for (; i < TOTAL_FINISHED_PROCESSES; ++i)
    {
        printf(" ( %i %i %i %i)", finished_process_list[i].A, finished_process_list[i].B,
            finished_process_list[i].C, finished_process_list[i].M);
    }
    printf("\n");
} // End of the print final function

/**
 * Prints out specifics for each process  (helper function, you may need to adjust variables accordingly)
 * @param process_list The original processes inputted, in array form
 */
void printProcessSpecifics(_process process_list[])
{
    uint32_t i = 0;
    printf("\n");
    for (; i < TOTAL_CREATED_PROCESSES; ++i)
    {
        printf("Process %i:\n", process_list[i].processID);
        printf("\t(A,B,C,M) = (%i,%i,%i,%i)\n", process_list[i].A, process_list[i].B,
            process_list[i].C, process_list[i].M);
        printf("\tFinishing time: %i\n", process_list[i].finishingTime);
        printf("\tTurnaround time: %i\n", process_list[i].finishingTime - process_list[i].A);
        printf("\tI/O time: %i\n", process_list[i].currentIOBlockedTime);
        printf("\tWaiting time: %i\n", process_list[i].currentWaitingTime);
        printf("\n");
    }
} // End of the print process specifics function

/**
 * Prints out the summary data (helper function, you may need to adjust variables accordingly)
 * process_list The original processes inputted in array form
 */
void printSummaryData(_process process_list[])
{
    uint32_t i = 0;
    double total_amount_of_time_utilizing_cpu = 0.0;
    double total_amount_of_time_io_blocked = 0.0;
    double total_amount_of_time_spent_waiting = 0.0;
    double total_turnaround_time = 0.0;
    uint32_t final_finishing_time = CURRENT_CYCLE - 1;
    for (; i < TOTAL_CREATED_PROCESSES; ++i)
    {
        total_amount_of_time_utilizing_cpu += process_list[i].currentCPUTimeRun;
        total_amount_of_time_io_blocked += process_list[i].currentIOBlockedTime;
        total_amount_of_time_spent_waiting += process_list[i].currentWaitingTime;
        total_turnaround_time += (process_list[i].finishingTime - process_list[i].A);
    }

    // Calculates the CPU utilisation
    double cpu_util = total_amount_of_time_utilizing_cpu / final_finishing_time;

    // Calculates the IO utilisation
    double io_util = (double)TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED / final_finishing_time;

    // Calculates the throughput (Number of processes over the final finishing time times 100)
    double throughput = 100 * ((double)TOTAL_CREATED_PROCESSES / final_finishing_time);

    // Calculates the average turnaround time
    double avg_turnaround_time = total_turnaround_time / TOTAL_CREATED_PROCESSES;

    // Calculates the average waiting time
    double avg_waiting_time = total_amount_of_time_spent_waiting / TOTAL_CREATED_PROCESSES;

    printf("Summary Data:\n");
    printf("\tFinishing time: %i\n", CURRENT_CYCLE - 1);
    printf("\tCPU Utilisation: %6f\n", cpu_util);
    printf("\tI/O Utilisation: %6f\n", io_util);
    printf("\tThroughput: %6f processes per hundred cycles\n", throughput);
    printf("\tAverage turnaround time: %6f\n", avg_turnaround_time);
    printf("\tAverage waiting time: %6f\n", avg_waiting_time);
} // End of the print summary data function


int compareByArrival(const void* a, const void* b) {
    _process* p1 = (_process*)a;
    _process* p2 = (_process*)b;

    if (p1->A != p2->A) {
        return p1->A - p2->A;
    }

    return p1->processID - p2->processID;
}

void sort_process_by_arrival(_process* process_list) {
    qsort(process_list, TOTAL_CREATED_PROCESSES, sizeof(_process), compareByArrival);
}



_process* parse_input_file(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open input file");
        exit(EXIT_FAILURE);
    }

    int n; //number of process inputs
    fscanf(fp, "%d", &n);
    TOTAL_CREATED_PROCESSES = n;

    _process* process_list = malloc(sizeof(_process) * n);
    if (!process_list) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        char ch;
        fscanf(fp, " %c", &ch);
        fscanf(fp, "%u %u %u %u", &process_list[i].A,&process_list[i].B,&process_list[i].C,&process_list[i].M);
        fscanf(fp, " %c", &ch);

        process_list[i].processID = i;
        process_list[i].remainingCPUTime = process_list[i].C;

        process_list[i].isBlocked = false;
        process_list[i].isRunning = false;
        process_list[i].isReady = false;
        process_list[i].isFinished = false;
        process_list[i].currentCPUTimeRun = 0;
        process_list[i].currentIOBlockedTime = 0;
        process_list[i].currentWaitingTime = 0;
        process_list[i].currentCPUBurst = 0;
        process_list[i].timeUntilUnblock = 0;
        process_list[i].remainingIOTime = 0;
        process_list[i].randOffset = 0;
    }

    fclose(fp);
    return process_list;
}

void fcfsScheduler(_process* process_list, _process* finished_list, FILE* random_num_file_ptr, bool verbose) {
    Queue ready_queue;
    initQueue(&ready_queue);
    _process* running_process = NULL;
    uint32_t last_cpu_burst = 0;
    while (TOTAL_FINISHED_PROCESSES < TOTAL_CREATED_PROCESSES) {
        // 1. Unblock any finished I/O processes
        for (int i = 0; i < TOTAL_CREATED_PROCESSES; i++) {
            _process* p = &process_list[i];
            if (p->isBlocked) {
                p->remainingIOTime--;
                if (p->remainingIOTime == 0) {
                    p->isBlocked = false;
                    p->isReady = true;
                    enqueue(&ready_queue, p);
                    printf("process io at cycle %d\n", CURRENT_CYCLE);
                }
            }
        }

        // 2. Check for new arrivals at this cycle
        for (int i = 0; i < TOTAL_CREATED_PROCESSES; i++) {
            _process* p = &process_list[i];
            if (p->A == CURRENT_CYCLE && !p->isReady && !p->isFinished) {
                p->isReady = true;
                enqueue(&ready_queue, p);
            }
        }

        // 3. If CPU is idle, schedule next ready process
        if (!running_process && !isEmpty(&ready_queue)) {
            running_process = dequeue(&ready_queue);
            running_process->isReady = false;
            running_process->isRunning = true;

            uint32_t cpu_burst = randomOS(running_process->B, running_process->randOffset,random_num_file_ptr);
            running_process->randOffset++;
            if (cpu_burst > (running_process->C - running_process->currentCPUTimeRun)) {
                cpu_burst = running_process->C - running_process->currentCPUTimeRun;
            }
            running_process->currentCPUBurst = cpu_burst;
            last_cpu_burst = cpu_burst;
        }

        // 4. Run currently running process
        if (running_process) {
            running_process->currentCPUBurst--;
            running_process->currentCPUTimeRun++;

            if (running_process->currentCPUBurst == 0) {
                if (running_process->currentCPUTimeRun == running_process->C) {
                    running_process->finishingTime = CURRENT_CYCLE + 1;
                    running_process->isFinished = true;
                    running_process->isRunning = false;
                    finished_list[TOTAL_FINISHED_PROCESSES++] = *running_process;
                    running_process = NULL;
                }
                else {
                    running_process->isBlocked = true;
                    running_process->isRunning = false;
                    running_process->remainingIOTime = running_process->M * last_cpu_burst;
                    running_process = NULL;
                }
            }
        }

        // 5. Update waiting time for all ready processes
        for (int i = 0; i < TOTAL_CREATED_PROCESSES; i++) {
            if (process_list[i].isReady) {
                process_list[i].currentWaitingTime++;
            }
        }

        // 6. Update I/O blocked time and global counter
        bool anyBlocked = false;
        for (int i = 0; i < TOTAL_CREATED_PROCESSES; i++) {
            if (process_list[i].isBlocked) {
                process_list[i].currentIOBlockedTime++;
                anyBlocked = true;
            }
        }
        if (anyBlocked) {
            TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED++;
        }

        CURRENT_CYCLE++;
    
    };
}

void roundRobinScheduler(_process* process_list, _process* finished_list, FILE* random_num_file_ptr, bool verbose) {
    Queue ready_queue;
    initQueue(&ready_queue);
    _process* running_process = NULL;

    uint32_t last_cpu_burst = 0;
    uint32_t quantumRemaining = TIME_QUANTUM;

    while (TOTAL_FINISHED_PROCESSES < TOTAL_CREATED_PROCESSES) {

        // 1. Unblock processes whose I/O is done
        for (int i = 0; i < TOTAL_CREATED_PROCESSES; i++) {
            _process* p = &process_list[i];
            if (p->isBlocked) {
                p->remainingIOTime--;
                if (p->remainingIOTime == 0) {
                    p->isBlocked = false;
                    p->isReady = true;
                    enqueue(&ready_queue, p);
                }
            }
        }

        // 2. Check for new arrivals
        for (int i = 0; i < TOTAL_CREATED_PROCESSES; i++) {
            _process* p = &process_list[i];
            if (p->A == CURRENT_CYCLE && !p->isFinished && !p->isReady) {
                p->isReady = true;
                enqueue(&ready_queue, p);
            }
        }

        // 3. If CPU is idle, pick new process
        if (!running_process && !isEmpty(&ready_queue)) {
            running_process = dequeue(&ready_queue);
            running_process->isReady = false;
            running_process->isRunning = true;

            if (running_process->currentCPUBurst == 0) {
                uint32_t cpu_burst = randomOS(running_process->B, running_process->randOffset, random_num_file_ptr);
                running_process->randOffset++;
                if (cpu_burst > (running_process->C - running_process->currentCPUTimeRun)) {
                    cpu_burst = running_process->C - running_process->currentCPUTimeRun;
                }
                running_process->currentCPUBurst = cpu_burst;
                last_cpu_burst = cpu_burst;
            }

            quantumRemaining = TIME_QUANTUM;
        }

        // 4. Execute running process
        if (running_process) {
            running_process->currentCPUBurst--;
            running_process->currentCPUTimeRun++;
            quantumRemaining--;

            if (running_process->currentCPUTimeRun == running_process->C) {
                // Finished
                running_process->finishingTime = CURRENT_CYCLE + 1;
                running_process->isFinished = true;
                running_process->isRunning = false;
                finished_list[TOTAL_FINISHED_PROCESSES++] = *running_process;
                running_process = NULL;
            }
            else if (running_process->currentCPUBurst == 0) {
                // Needs I/O
                running_process->isBlocked = true;
                running_process->isRunning = false;
                running_process->remainingIOTime = running_process->M * last_cpu_burst;
                running_process = NULL;
            }
            else if (quantumRemaining == 0) {
                // Preempt
                running_process->isRunning = false;
                running_process->isReady = true;
                enqueue(&ready_queue, running_process);
                running_process = NULL;
            }
        }

        // 5. Update waiting time
        for (int i = 0; i < TOTAL_CREATED_PROCESSES; i++) {
            if (process_list[i].isReady) {
                process_list[i].currentWaitingTime++;
            }
        }

        // 6. Update I/O blocked time and global tracker
        bool anyBlocked = false;
        for (int i = 0; i < TOTAL_CREATED_PROCESSES; i++) {
            if (process_list[i].isBlocked) {
                process_list[i].currentIOBlockedTime++;
                anyBlocked = true;
            }
        }
        if (anyBlocked) {
            TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED++;
        }

        CURRENT_CYCLE++;
    }
}

void sjfScheduler(_process* process_list, _process* finished_list, FILE* random_num_file_ptr, bool verbose) {
    _process* running_process = NULL;
    uint32_t last_cpu_burst = 0;

    while (TOTAL_FINISHED_PROCESSES < TOTAL_CREATED_PROCESSES) {
        // 1. Unblock finished I/O
        for (int i = 0; i < TOTAL_CREATED_PROCESSES; i++) {
            _process* p = &process_list[i];
            if (p->isBlocked) {
                p->remainingIOTime--;
                if (p->remainingIOTime == 0) {
                    p->isBlocked = false;
                    p->isReady = true;
                }
            }
        }

        // 2. Check for arrivals
        for (int i = 0; i < TOTAL_CREATED_PROCESSES; i++) {
            _process* p = &process_list[i];
            if (p->A == CURRENT_CYCLE && !p->isFinished && !p->isReady) {
                p->isReady = true;
            }
        }

        // 3. If CPU idle, pick ready process with shortest remaining job
        if (!running_process) {
            _process* shortest = NULL;
            for (int i = 0; i < TOTAL_CREATED_PROCESSES; i++) {
                _process* p = &process_list[i];
                if (p->isReady && !p->isFinished) {
                    uint32_t remaining = p->C - p->currentCPUTimeRun;
                    if (!shortest || remaining < (shortest->C - shortest->currentCPUTimeRun)) {
                        shortest = p;
                    }
                }
            }

            if (shortest) {
                running_process = shortest;
                running_process->isReady = false;
                running_process->isRunning = true;

                uint32_t cpu_burst = randomOS(running_process->B, running_process->randOffset, random_num_file_ptr);
                running_process->randOffset++;
                if (cpu_burst > (running_process->C - running_process->currentCPUTimeRun)) {
                    cpu_burst = running_process->C - running_process->currentCPUTimeRun;
                }
                running_process->currentCPUBurst = cpu_burst;
                last_cpu_burst = cpu_burst;
            }
        }

        // 4. Execute current process
        if (running_process) {
            running_process->currentCPUBurst--;
            running_process->currentCPUTimeRun++;

            if (running_process->currentCPUTimeRun == running_process->C) {
                // Done
                running_process->finishingTime = CURRENT_CYCLE + 1;
                running_process->isFinished = true;
                running_process->isRunning = false;
                finished_list[TOTAL_FINISHED_PROCESSES++] = *running_process;
                running_process = NULL;
            }
            else if (running_process->currentCPUBurst == 0) {
                // Needs I/O
                running_process->isBlocked = true;
                running_process->isRunning = false;
                running_process->remainingIOTime = running_process->M * last_cpu_burst;
                running_process = NULL;
            }
        }

        // 5. Waiting time for ready processes
        for (int i = 0; i < TOTAL_CREATED_PROCESSES; i++) {
            if (process_list[i].isReady) {
                process_list[i].currentWaitingTime++;
            }
        }

        // 6. I/O time
        bool anyBlocked = false;
        for (int i = 0; i < TOTAL_CREATED_PROCESSES; i++) {
            if (process_list[i].isBlocked) {
                process_list[i].currentIOBlockedTime++;
                anyBlocked = true;
            }
        }
        if (anyBlocked) {
            TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED++;
        }

        CURRENT_CYCLE++;
    }
}



#endif // !PROCESS_H