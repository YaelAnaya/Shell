//
// Created by yaelao on 5/30/23.
//

#ifndef SHELL_PROCESS_H
#define SHELL_PROCESS_H

#include "Queue.h"

enum ProcessState {
    NEW, READY, WAITING, RUNNING, TERMINATED
};

typedef struct {
    int pid;
    enum ProcessState state;
    int burst_time;
    int waiting_time;
    int turn_around_time;
    int t_time;
    int size;
} Process;

Process *make_process(int pid, int burst_time, int memory_blocks);
int compare_process(void *data1, void *data2);
void print_process(void *data);
void first_come_first_served(Queue *queue);
void shortest_job_first(Queue *queue);
int get_shortest_index(Queue *queue);
void round_robin(Queue *queue, int quantum);
Process *get_process(Queue *queue, int pid);
void free_process(Process *process);
void kill_process(Queue *queue, int pid);

#endif //SHELL_PROCESS_H
