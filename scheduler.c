#include "process.h"

/**
 * The magic starts from here
 */
int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    bool verbose = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
            verbose = true;
        }
    }

    _process* process_list = parse_input_file(argv[1]);
    _process* finished_list = malloc(sizeof(_process) * TOTAL_CREATED_PROCESSES);
    sort_process_by_arrival(process_list); //sort processes by arrival

    FILE* random_num_file_ptr = fopen("random-numbers", "r");
    if (!random_num_file_ptr) {
        perror("Failed to open random-number file");
        exit(1);
    }

    printf("######################### START OF FIRST COME FIRST SERVE #########################\n");
    printStart(process_list);
    fcfsScheduler(process_list, finished_list, random_num_file_ptr,verbose);
    
    
    printFinal(process_list);
    printProcessSpecifics(process_list);
    printSummaryData(process_list);
    printf("######################### END OF FIRST COME FIRST SERVE #########################\n");

    printf("######################### START OF ROUND ROBIN #########################\n");
    TOTAL_FINISHED_PROCESSES = 0;
    CURRENT_CYCLE = 0;
    TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED = 0;
    //reset
    for (int i = 0; i < TOTAL_CREATED_PROCESSES; i++) {
        _process* p = &process_list[i];
        p->isReady = false;
        p->isBlocked = false;
        p->isRunning = false;
        p->isFinished = false;
        p->currentCPUTimeRun = 0;
        p->currentIOBlockedTime = 0;
        p->currentWaitingTime = 0;
        p->remainingIOTime = 0;
        p->currentCPUBurst = 0;
        p->finishingTime = 0;
        p->randOffset = 0;
    }
    printStart(process_list);
    roundRobinScheduler(process_list, finished_list, random_num_file_ptr, true);
    printFinal(finished_list);
    printProcessSpecifics(finished_list);
    printSummaryData(finished_list);
    printf("######################### END OF ROUND ROBIN #########################\n");

    printf("######################### START OF SHORTEST JOB FIRST #########################\n");
    TOTAL_FINISHED_PROCESSES = 0;
    CURRENT_CYCLE = 0;
    TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED = 0;
    //reset
    for (int i = 0; i < TOTAL_CREATED_PROCESSES; i++) {
        _process* p = &process_list[i];
        p->isReady = false;
        p->isBlocked = false;
        p->isRunning = false;
        p->isFinished = false;
        p->currentCPUTimeRun = 0;
        p->currentIOBlockedTime = 0;
        p->currentWaitingTime = 0;
        p->remainingIOTime = 0;
        p->currentCPUBurst = 0;
        p->finishingTime = 0;
        p->randOffset = 0;
    }
    printStart(process_list);
    sjfScheduler(process_list, finished_list, random_num_file_ptr, verbose);
    printFinal(finished_list);
    printProcessSpecifics(finished_list);
    printSummaryData(finished_list);
    printf("######################### END OF SHORTEST JOB FIRST #########################\n");

    
    
    


    free(process_list);
    free(finished_list);
    fclose(random_num_file_ptr);

    return 0;
} 