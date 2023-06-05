//
// Created by yaelao on 5/30/23.
//

#include "Memory.h"

/**
 * Esta funcion inicializa la memoria con un bloque de tamaño MAX_SIZE.
 * */
void init_memory() {
    memory = (Memory *) malloc(sizeof(Memory));
    memory->total_size = MAX_SIZE;
    memory->remaining_size = MAX_SIZE;
    memory->blocks = create_list();

    MemoryBlock *block = (MemoryBlock *) malloc(sizeof(MemoryBlock));
    block->size = MAX_SIZE;
    block->base = 0;
    block->limit = MAX_SIZE - 1;
    block->remaining_size = MAX_SIZE;
    block->process = NULL;
    append(memory->blocks, block, sizeof(MemoryBlock));

}

/**
 * Esta funcion obtiene el limite de un bloque de memoria.
 * @param block_index El indice del bloque.
 * @return El limite del bloque.
 * */
int get_limit_from(int block_number) {
    MemoryBlock *block = get_at(memory->blocks, block_number);
    return block->process->size + block->base - 1;
}

/**
 * Obtiene el tamaño de un bloque de memoria.
 * @param block_number El numero del bloque de memoria.
 * @return El tamaño del bloque de memoria.
 * */
int get_size_from(int block_number) {
    MemoryBlock *block = get_at(memory->blocks, block_number);
    return (block->limit + 1) - block->base;
}

/**
 * Obtiene la memoria restante de un bloque de memoria.
 * @param block_number El numero del bloque de memoria.
 * @return La memoria restante del bloque de memoria.
 * */
int get_remaining_memory_from(int block_number) {
    MemoryBlock *block = (MemoryBlock *) get_at(memory->blocks, block_number);
    Process *process = block->process;

    if (process == NULL) {
        return block->size;
    } else {
        return block->size - process->size;
    }
}

/**
 * Esta funcion crea un bloque de memoria.
 * @param block_number El numero del bloque de memoria.
 * @param base La base del bloque de memoria.
 * @param limit El limite del bloque de memoria.
 * */
void make_memory_block(int block_number, int base, int limit) {
    MemoryBlock *block = (MemoryBlock *) malloc(sizeof(MemoryBlock));
    block->base = base;
    block->limit = limit;
    block->remaining_size = limit - base;
    block->size = block->remaining_size;
    block->process = NULL;
    add_at(memory->blocks, block,
           block_number, sizeof(MemoryBlock));
}

/**
 * Esta funcion asigna un proceso a un bloque de memoria.
 * @param pid El identificador del proceso.
 * @param size El tamaño del proceso.
 * @param block_number El numero del bloque de memoria.
 * @return true si se asigno el proceso, false en caso contrario.
 * */
bool assign_to_block(int block_index, Process *process) {
    long remaining_memory = get_remaining_memory_from(block_index);

    /* Si el tamaño del proceso es mayor a la memoria restante
     * del bloque de memoria, entonces no se puede asignar el proceso. */
    if (remaining_memory < process->size) {
        return false;
    }
    // Se crea el proceso y se asigna al bloque de memoria.
//    Process *process = (Process *) malloc(sizeof(Process));
//    process->pid = malloc(sizeof(char) * strlen(pid));
//    strcpy(process->pid, pid);
//    process->size = size;
    MemoryBlock *block = (MemoryBlock *) get_at(memory->blocks, block_index);
    block->process = process;
    block->process->state = READY;

    // Se crea un nuevo bloque de memoria si hay suficiente memoria restante.
    if (remaining_memory > 0) {
        int new_block_limit = block->limit;
        block->limit = get_limit_from(block_index);
        block->size = get_size_from(block_index);
        block->remaining_size = get_remaining_memory_from(block_index);
        int new_block_base = block->limit + 1;

        make_memory_block(block_index + 1,
                          new_block_base, new_block_limit);
    }
    return true;
}

/**
 * Esta funcion libera la memoria de un proceso.
 * @param pid El identificador del proceso.
 * @return true si se libero la memoria, false en caso contrario.
 * */
bool free_memory(int pid) {
    for (int i = 0; i < memory->blocks->size; i++) {
        MemoryBlock *block = get_at(memory->blocks, i);
        if (block->process != NULL && block->process->pid == pid) {
            block->process->state = NEW;
            block->process = NULL;
            block->remaining_size = block->size;
            return true;
        }
    }
    return false;
}

/**
 * Esta funcion compacta la memoria.
 * */
void compact_memory() {
    // Se recorre la lista de bloques de memoria.
    for (int i = 0; i < memory->blocks->size; i++) {
        long remaining_memory = get_remaining_memory_from(i);
        // Si hay memoria restante, se compacta.
        if (remaining_memory > 0) {
            // Se verifica si se puede compactar con el bloque anterior.
            if (i > 0) {
                if (get_remaining_memory_from(i - 1) > 0) {
                    MemoryBlock *block = get_at(memory->blocks, i);
                    MemoryBlock *previous_block = get_at(memory->blocks, i - 1);
                    block->size = block->size + previous_block->size;
                    block->base = previous_block->base;
                    block->remaining_size = block->size;
                    remove_element_at(memory->blocks, i - 1);
                }
            }
            // Se verifica si se puede compactar con el bloque siguiente.
            if (i < memory->blocks->size - 1) {
                if (get_remaining_memory_from(i + 1) > 0) {
                    MemoryBlock *block = get_at(memory->blocks, i);
                    MemoryBlock *next_block = get_at(memory->blocks, i + 1);
                    block->size = block->size + next_block->size;
                    block->limit = next_block->limit;
                    block->remaining_size = block->size;
                    remove_element_at(memory->blocks, i + 1);
                }
            }
        }
    }
}

/**
 * Esta funcion simula el algoritmo de asignacion de memoria First Fit.
 * @param pid El identificador del proceso.
 * @param size El tamaño del proceso.
 * */
void best_fit(Process *process) {
    MemoryBlock *best_block = memory->blocks->head->data;
    int best_block_index = 0;

    for (int i = 0; i < memory->blocks->size; i++) {
        MemoryBlock *block = get_at(memory->blocks, i);
        /* Si el bloque de memoria no tiene un proceso asignado y
         * el tamaño del bloque es mayor al tamaño del proceso,
         * entonces ese es el mejor bloque de memoria.
         * */
        if (block->process == NULL && block->remaining_size >= best_block->remaining_size) {
            best_block = block;
            best_block_index = i;
        }
    }

    for (int i = 0; i < memory->blocks->size; i++) {
        MemoryBlock *block = get_at(memory->blocks, i);
        /* Si el espacio restante del bloque de memoria es menor al espacio restante
         * del mejor bloque de memoria y el espacio restante del bloque de memoria
         * es mayor al tamaño del proceso, entonces ese es el mejor bloque de memoria.
         * */
        if (block->remaining_size < best_block->remaining_size
            && block->remaining_size > process->size) {
            best_block = block;
            best_block_index = i;
        }
    }
    // Se asigna el proceso al mejor bloque de memoria.
    if (assign_to_block(best_block_index, process) == true) {
        printf("Process %d assigned to block %d\n",
               process->pid, best_block_index + 1);
    } else {
        printf("Process %d could not be assigned to block %d\n",
               process->pid, best_block_index + 1);
    }
}

/**
 * Esta funcion simula el algoritmo de asignacion de memoria Worst Fit.
 * @param pid El identificador del proceso.
 * @param size El tamaño del proceso.
 * */
void worst_fit(Process *process) {
    MemoryBlock *worst_block = memory->blocks->head->data;
    int worst_block_index = 0;
    for (int i = 0; i < memory->blocks->size; i++) {
        MemoryBlock *block = get_at(memory->blocks, i);
        /* Si el espacio restante del bloque de memoria es mayor al del
         * peor bloque de memoria y el espacio restante del bloque de memoria
         * es mayor al tamaño del proceso, entonces ese es el peor bloque de memoria.
         * */
        if (block->remaining_size > worst_block->remaining_size && block->remaining_size > process->size) {
            worst_block = block;
            worst_block_index = i;
        }
    }
    // Se asigna el proceso al mejor bloque de memoria.
    if (assign_to_block(worst_block_index, process) == true) {
        printf("Process %d assigned to block %d\n",
               process->pid, worst_block_index + 1);
    } else {
        printf("Process %d could not be assigned to block %d\n",
               process->pid, worst_block_index + 1);
    }
}

/**
 * Esta funcion simula el algoritmo de asignacion de memoria First Fit.
 * @param pid El identificador del proceso.
 * @param size El tamaño del proceso.
 * */
void first_fit(Process *process) {
    // Si la lista de bloques de memoria no esta vacia.
    if (!is_list_empty(memory->blocks)) {
        int i = 0;
        MemoryBlock *block = get_at(memory->blocks, i);
        // Se busca el primer bloque de memoria que tenga espacio suficiente.
        while (block->remaining_size < process->size || i < memory->blocks->size - 1) {
            block = get_at(memory->blocks, ++i);
        }
        // Se asigna el proceso al bloque de memoria.
        if (assign_to_block(i, process) == true) {
            printf("Process %d assigned to block %d\n",
                   process->pid, i);
        } else {
            printf("Memory is full\n");
        }
    }
}

/**
 * Esta funcion asigna un proceso a un bloque de memoria,
 * utilizando el algoritmo de asignacion de memoria especificado.
 * @param pid El identificador del proceso.
 * @param size El tamaño del proceso.
 * @param fit El algoritmo de asignacion de memoria.
 * */
void assign_memory(Process *process, char *fit) {
    if (strcmp(fit, "ff") == 0) {
        first_fit(process);
    } else if (strcmp(fit, "bf") == 0) {
        best_fit(process);
    } else if (strcmp(fit, "wf") == 0) {
        worst_fit(process);
    } else{
        printf("Invalid fit\n");
    }
}

/**
 * Esta funcion se utiliza para reportar el estado de la memoria.
 * Se imprime una tabla con los bloques de memoria y su estado.
 * */
void report_memory() {
    char *columns[] = {"Block#", "Process ID",
                       "Base", "Limit", "Available space", "Size"};

    printf("%8s %12s %7s %8s %18s %7s\n",
           columns[0], columns[1], columns[2], columns[3], columns[4], columns[5]);

    for (int i = 0; i < memory->blocks->size; i++) {
        MemoryBlock *block = get_at(memory->blocks, i);

        if (block->process == NULL) {
            printf("%3d %11s %11d %10d %8d %18d\n",
                   i + 1, "Free", block->base, block->limit,
                   block->remaining_size, block->size);
            continue;
        }

        printf("%3d %11d %11d %10d %8d %18d\n",
               i + 1, block->process->pid, block->base, block->limit,
               block->remaining_size, block->size);
    }
}