//
// Created by yaelao on 5/30/23.
//

#ifndef SHELL_LIST_H
#define SHELL_LIST_H
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "Queue.h"


/**
 * Estructura que representa una lista.
 * @param head El primer nodo de la lista.
 * @param tail El ultimo nodo de la lista.
 * @param size El numero de nodos en la lista.
 * */
typedef struct {
    Node *head;
    Node *tail;
    int size;
} List;

List *create_list();
void clear_list(List *list);
void append(List *list, void *data, size_t type_size);
void add_at(List *list, void *data, int index, size_t type_size);
void *remove_node_list(List *list, void *data, int (*cmp_func)(void *, void *));
int index_of(List *list, void *data, int (*cmp_func)(void *, void *));
void *remove_element_at(List *list, int index);
void *get_at(List *list, int index);
Node *create_node_list(size_t type_size);
bool is_list_empty(List *list);
void print_list(List *list, void (*p_fun)(void*));
#endif //SHELL_LIST_H
