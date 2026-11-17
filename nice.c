/*
Instructions:

1)Save as nice.c

2)Compile: "gcc nice.c -o nice"
3) Run: "./nice"
*/

#include <stdio.h>

#define NUM_TASKS 3
#define TOTAL_CPU_TIME 1000.0

typedef struct {
    char name;
    int nice;
    double weight;
    double cpu_share;
    double allocated_time;
} Task;

// Simplified nice-to-weight table
double get_weight(int nice) {
    switch (nice) {
        case -5: return 3121;
        case  0: return 1024;
        case  5: return 335;
        case 19: return 15;
        default: return 1024;
    }
}

int main(void) {

    Task tasks[NUM_TASKS] = {
        {'A', -5, 0, 0, 0},
        {'B',  0, 0, 0, 0},
        {'C',  5, 0, 0, 0}
    };

    double total_weight = 0;

    // Calculate each task's weight
    for (int i = 0; i < NUM_TASKS; i++) {
        tasks[i].weight = get_weight(tasks[i].nice);
        total_weight += tasks[i].weight;
    }

    printf("========== NICE VALUE SIMULATION ==========\n\n");

    // Calculate proportional CPU allocation
    for (int i = 0; i < NUM_TASKS; i++) {

        tasks[i].cpu_share =
            (tasks[i].weight / total_weight) * 100;

        tasks[i].allocated_time =
            (tasks[i].cpu_share / 100) * TOTAL_CPU_TIME;

        printf("Task %c | Nice: %2d | Weight: %.0f\n",
               tasks[i].name,
               tasks[i].nice,
               tasks[i].weight);

        printf("CPU Share: %.2f%% | CPU Time: %.2f\n\n",
               tasks[i].cpu_share,
               tasks[i].allocated_time);
    }

    printf("Total CPU time: %.0f units\n", TOTAL_CPU_TIME);

    return 0;
}

