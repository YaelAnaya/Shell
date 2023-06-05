//
// Created by yaelao on 5/30/23.
//

#ifndef SHELL_PROMPT_H
#define SHELL_PROMPT_H
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Queue.h"
#include "List.h"
#include "Process.h"
#include "Memory.h"

#define READ_END 0
#define WRITE_END 1

#define MAX_SIZE 1024

static Queue *process_queue;

enum Option {
    ALLOC, FREE, COMPACT, STATE,
    MKPS, LSP, KILL, RR, FCFS, SJF
};

typedef struct {
    char *key;
    enum Option value;
} Pair;

// Tabla hash que almacena los comandos de entrada.
static Pair lookup_table[] = {
        {"alloc", ALLOC},
        {"free", FREE},
        {"compact", COMPACT},
        {"state", STATE},
        {"mkps", MKPS},
        {"lsp", LSP},
        {"kill", KILL},
        {"rr", RR},
        {"fcfs", FCFS},
        {"sjf", SJF},
};


void show_prompt();
char **split_args_execvp(char *args);
char **split_args(char *args);
void init_shell();


#endif //SHELL_PROMPT_H
