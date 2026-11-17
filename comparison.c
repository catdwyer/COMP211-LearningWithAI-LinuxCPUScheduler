
#include <stdio.h>

#define NUM_TASKS 3
#define NICE_0_LOAD 1024.0

typedef struct {
    char name;
    double weight;
    double vruntime;
    double slice;
} Task;

void run_cfs(Task tasks[]) {
    printf("\n========== CFS SCHEDULER ==========\n");

    for (int step = 0; step < 9; step++) {
        int selected = 0;

        // Find task with smallest vruntime
        for (int i = 1; i < NUM_TASKS; i++) {
            if (tasks[i].vruntime <
                tasks[selected].vruntime) {
                selected = i;
            }
        }

        Task *t = &tasks[selected];

        printf("Step %d: Task %c runs | vruntime = %.2f\n",
               step + 1, t->name, t->vruntime);

        // CFS uses a fixed teaching time quantum
        t->vruntime +=
            5.0 * NICE_0_LOAD / t->weight;
    }
}

double weighted_average(Task tasks[]) {
    double total_weight = 0;
    double total_runtime = 0;

    for (int i = 0; i < NUM_TASKS; i++) {
        total_weight += tasks[i].weight;
        total_runtime +=
            tasks[i].weight * tasks[i].vruntime;
    }

    return total_runtime / total_weight;
}

void run_eevdf(Task tasks[]) {
    printf("\n========== EEVDF SCHEDULER ==========\n");

    for (int step = 0; step < 9; step++) {

        double avg = weighted_average(tasks);
        int selected = -1;
        double earliest_deadline = 1e30;

        for (int i = 0; i < NUM_TASKS; i++) {

            // Calculate task eligibility
            double lag = avg - tasks[i].vruntime;

            if (lag >= -0.000001) {

                // Calculate virtual deadline
                double deadline =
                    tasks[i].vruntime +
                    tasks[i].slice *
                    NICE_0_LOAD / tasks[i].weight;

                if (deadline < earliest_deadline) {
                    earliest_deadline = deadline;
                    selected = i;
                }
            }
        }

        if (selected == -1) {
            printf("No eligible task found.\n");
            break;
        }

        Task *t = &tasks[selected];

        printf("Step %d: Task %c runs | "
               "vruntime = %.2f | deadline = %.2f\n",
               step + 1,
               t->name,
               t->vruntime,
               earliest_deadline);

        // Run task for its requested slice
        t->vruntime +=
            t->slice * NICE_0_LOAD / t->weight;
    }
}

int main(void) {

    // Identical starting tasks for both schedulers
    Task cfs_tasks[NUM_TASKS] = {
        {'A', 2048, 0, 10},
        {'B', 1024, 0, 5},
        {'C',  512, 0, 2}
    };

    Task eevdf_tasks[NUM_TASKS] = {
        {'A', 2048, 0, 10},
        {'B', 1024, 0, 5},
        {'C',  512, 0, 2}
    };

    run_cfs(cfs_tasks);
    run_eevdf(eevdf_tasks);

    return 0;
}

