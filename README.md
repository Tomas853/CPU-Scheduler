### CPU Scheduler

A CPU scheduling simulator in C that models process execution and evaluates the performance of scheduling algorithms of the following: First-Come First-Serve (FCFS), Round Robin (RR), and Shortest Job First (SJF).

The simulator manages process state transitions between Ready, Running, Blocked, and Terminated states while tracking key operating system performance metrics such as CPU utilization, I/O utilization, waiting time, turnaround time, and throughput.

#### Key Concepts Demonstrated
- Operating System Process Scheduling
- Systems Programming in C
- Queue-Based Data Structures
- Performance Analysis and Simulation
- Process State Management
- Scheduling Algorithms

#### Technical Highlights
- Implemented FCFS, Round Robin, and SJF scheduling policies
- Simulated CPU bursts, I/O bursts, and process lifecycle management
- Calculated scheduling performance metrics and generated summary statistics
- Applied deterministic tie-breaking rules to ensure reproducible results

#### Technologies
C, Linux, Operating Systems, Data Structures

#### Ways to run
`make scheduler` or just run the scheduler executable after downloading this repository
use either scheduler test runs from the three listed on the 'makefile' file and it will run each scheduler with each different input

Documentation: [Documentation](./pa2_gebrewold.pdf)
