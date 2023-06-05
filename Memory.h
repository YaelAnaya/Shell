//
// Created by yaelao on 5/30/23.
//

#ifndef SHELL_MEMORY_H
#define SHELL_MEMORY_H

#include "Process.h"
#include "List.h"

#define MAX_SIZE 1024

typedef struct {
    int size;
    int remaining_size;
    int base;
    int limit;
    Process *process;
} MemoryBlock;

typedef struct {
    int total_size;
    int remaining_size;
    List *blocks;
} Memory;

static Memory *memory;

void init_memory();
int get_limit_from(int block_index);
int get_size_from(int block_index);
int get_remaining_memory_from(int block_index);
void make_memory_block(int block_index, int base, int limit);
bool assign_to_block(int block_index, Process *process);
bool free_memory(int pid);
void compact_memory();
void best_fit(Process *process);
void worst_fit(Process *process);
void first_fit(Process *process);
void report_memory();
void assign_memory(Process *process, char *fit);
#endif //SHELL_MEMORY_H
