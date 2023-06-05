//
// Created by yaelao on 5/30/23.
//

#include "Process.h"
#include "Memory.h"

/**
 * Esta funcion crea un proceso y lo agrega a la queue de procesos creados.
 * @param queue La queue donde se agregara el proceso.
 * @param pid El id del proceso.
 * @param name El nombre del proceso.
 * @param burst_time El tiempo de rafaga del proceso.
 * */
Process *make_process(int pid, int burst_time, int memory_blocks) {
    Process *process = (Process *) malloc(sizeof(Process));
    process->pid = pid;
    process->burst_time = burst_time;
    process->size = memory_blocks;
    process->state = NEW;
    process->waiting_time = 0;
    process->turn_around_time = 0;
    process->t_time = 0;
    return process;
}

/**
 * Esta funcion compara dos procesos por su pid.
 * @param data1 El primer proceso.
 * @param data2 El segundo proceso.
 * @return Un numero negativo si el pid del primer proceso es menor que el del segundo,
 *        un numero positivo si el pid del primer proceso es mayor que el del segundo,
 *        cero si los pid son iguales.
 * */
int compare_process(void *data1, void *data2) {
    Process *process1 = (Process *) data1;
    Process *process2 = (Process *) data2;
    return process1->pid - process2->pid;
}

char *get_state(Process *process) {
    switch (process->state) {
        case NEW:
            return "NEW";
        case READY:
            return "READY";
        case RUNNING:
            return "RUNNING";
        case WAITING:
            return "WAITING";
        case TERMINATED:
            return "TERMINATED";
        default:
            return "UNKNOWN";
    }
}

/**
 * Esta funcion imprime el contenido de un proceso.
 * @param data El proceso a imprimir.
 * */
void print_process(void *data) {
    Process *process = (Process *) data;

    printf("PID: %d, Burst Time: %d, Memory Blocks: %d, State: %s\n",
           process->pid,
           process->burst_time,
           process->size,
           get_state(process)
    );
}


/**
 * Esta funcion simula el algoritmo de planificacion de procesos
 * First Come First Served.
 * @param queue La queue de procesos.
 * */
void first_come_first_served(Queue *queue) {
    Node *current = queue->head;
    int current_time = 0, num_process = queue->size;
    float total_wt = 0.0f, total_tat = 0.0f;

    if (is_queue_empty(queue)) {
        printf("Process queue is empty\n");
        return;
    }

    while (current != NULL) {
        Process *process = (Process *) current->data;
        if (process->state == READY) {
            // Se asigna el wt y el tat al proceso actual.
            process->waiting_time = current_time;
            process->turn_around_time = process->waiting_time + process->burst_time;

            printf("Process [%d] enter to CPU, at: %d ms\n",
                   process->pid, process->waiting_time);

            current_time += process->burst_time;
            total_wt += process->waiting_time;
            total_tat += process->turn_around_time;

            printf("Process [%d] exit from CPU, after %d ms\n\n",
                   process->pid, process->burst_time);

            // El proceso pasa de Ready a Terminated.
            process->state = TERMINATED;
            free_memory(process->pid);
            current = (Node *) current->next;
        }
    }

    printf("Average waiting time: %.2f\n",
           total_wt / (float) num_process);
    printf("Average turn around time: %.2f\n",
           total_tat / (float) num_process);
}

/**
 * Esta funcion obtiene el indice del proceso con el menor burst time.
 * @param queue La queue de procesos.
 * @return El indice del proceso con el menor burst time.
 * */
int get_shortest_index(Queue *queue) {
    Node *current = queue->head;
    int shortest_index = 0, index = 0;
    int shortest_burst_time = ((Process *) current->data)->burst_time;

    while (current != NULL) {
        Process *process = (Process *) current->data;
        if (process->burst_time < shortest_burst_time && process->state == READY) {
            shortest_burst_time = process->burst_time;
            shortest_index = index;
        }
        current = (Node *) current->next;
        index++;
    }

    return shortest_index;
}

/**
 * Esta funcion simula el algoritmo de planificacion de procesos
 * Shortest Job First.
 * @param queue La queue de procesos.
 * */
void shortest_job_first(Queue *queue) {
    if (is_queue_empty(queue)) {
        printf("Process queue is empty\n");
        return;
    }

    // se ordena la lista de procesos por burst time de menor a mayor.
    sort_queue(queue, compare_process);
    first_come_first_served(queue);
}

/**
 * Esta funcion simula el algoritmo de planificacion de procesos
 * Round Robin.
 * @param queue La queue de procesos.
 * */
void round_robin(Queue *queue, int quantum) {
    int current_time = 0, num_process = queue->size;
    float total_wt = 0.0f, total_tat = 0.0f;
    int ready_processes = 0, terminated_processes = 0;
    Node *current = queue->head;

    int remaining_burst[num_process], waiting_time[num_process],
            turn_around_time[num_process], iteration[num_process];

    if (is_queue_empty(queue)) {
        printf("Process queue is empty\n");
        return;
    }

    for (int i = 0; i < num_process; i++) {
        Process *process = (Process *) current->data;
        if (process->state == READY) {
            ready_processes++;
        }
        remaining_burst[i] = process->burst_time;
        waiting_time[i] = 0;
        turn_around_time[i] = 0;
        iteration[i] = 0;
        current = (Node *) current->next;
    }
    printf("\nTa\t\tProcess\t\tState\t\tRemain\t\tW Time\t\tTaT\n");

    do {
        current = queue->head;
        for (int i = 0; i < num_process; i++) {
            Process *process = (Process *) current->data;
            if (process->state == READY || process->state == WAITING) {
                if (remaining_burst[i] > 0) {
                    if (remaining_burst[i] > quantum) {
                        waiting_time[i] += current_time - turn_around_time[i];
                        printf("%d\t\tps %d\t\tenter\t\t%d/%d\t\t%d\t\t%d\n", current_time, process->pid,
                               remaining_burst[i], process->burst_time, waiting_time[i], iteration[i]);
                        process->state = RUNNING;
                        current_time += quantum;
                        remaining_burst[i] -= quantum;
                        turn_around_time[i] = current_time;
                        printf("%d\t\tps %d\t\texit\t\t%d/%d\t\t%d\t\t%d\n", current_time, process->pid,
                               remaining_burst[i], process->burst_time, waiting_time[i], iteration[i]);
                        process->state = WAITING;

                    } else {
                        waiting_time[i] += current_time - turn_around_time[i];
                        printf("%d\t\tps %d\t\tenter\t\t%d/%d\t\t%d\t\t%d\n", current_time, process->pid,
                               remaining_burst[i], process->burst_time, waiting_time[i], iteration[i]);
                        current_time += remaining_burst[i];
                        remaining_burst[i] = 0;
                        iteration[i] = current_time;
                        total_wt += waiting_time[i];
                        total_tat += iteration[i];
                        printf("%d\t\tps %d\t\texit\t\t%d/%d\t\t%d\t\t%d\n", current_time, process->pid,
                               remaining_burst[i], process->burst_time, waiting_time[i], iteration[i]);
                        process->state = TERMINATED;
                        terminated_processes++;
                        free_memory(process->pid);
                    }
                }

            }
            current = (Node *) current->next;
        }
    } while (ready_processes != terminated_processes);

    if (total_wt != 0) {
        printf("Average waiting time: %.2f\n",
               total_wt / (float) num_process);
        printf("Average turn around time: %.2f\n",
               total_tat / (float) num_process);
    }

}

/**
* Esta función obtiene la referencia de un proceso en la queue.
*/

Process *get_process(Queue *queue, int pid) {
    Node *current = queue->head;
    while (current != NULL) {
        Process *process = (Process *) current->data;
        if (process->pid == pid) {
            return process;
        }
        current = (Node *) current->next;
    }
    return NULL;
}

void kill_process(Queue *queue, int pid) {
    Process *process = get_process(queue, pid);
    if (process != NULL) {
        process->state = TERMINATED;
        free(dequeue_node(queue, process, compare_process));
        printf("Process [%d] killed\n", pid);
        return;
    }
    printf("Process [%d] doesn't exist\n", pid);
}
