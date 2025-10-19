#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Headers as needed

typedef enum { false, true } bool;        // Allows boolean types in C

/* Defines a job struct */
typedef struct Process {
    uint32_t A;                         // A: Arrival time of the process
    uint32_t B;                         // B: Upper Bound of CPU burst times of the given random integer list
    uint32_t C;                         // C: Total CPU time required
    uint32_t M;                         // M: Multiplier of CPU burst time
    uint32_t processID;                 // The process ID given upon input read

    uint32_t remainingCPUTime;
    uint32_t currentCPUTimeRun;
    uint32_t currentIOBlockedTime;
    uint32_t currentWaitingTime;
    uint32_t finishingTime;
    
    uint32_t currentBurstRemaining;
    uint32_t timeUntilUnblock;
    uint32_t isBlocked;
    uint32_t isRunning;
    uint32_t isTerminated;
} _process;

uint32_t TOTAL_CREATED_PROCESSES = 0;   // The total number of processes constructed

// Additional variables as needed


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
uint32_t randomOS(uint32_t upper_bound, uint32_t process_indx, FILE* random_num_file_ptr)
{
    char str[20];

    uint32_t unsigned_rand_int = (uint32_t)getRandNumFromFile(SEED_VALUE + process_indx, random_num_file_ptr);
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
        process_list[i].isTerminated = false;
        process_list[i].currentCPUTimeRun = 0;
        process_list[i].currentIOBlockedTime = 0;
        process_list[i].currentWaitingTime = 0;
        process_list[i].finishingTime = 0;
    }

    fclose(fp);
    return process_list;
}

#endif // !PROCESS_H