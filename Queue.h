//
// Created by yaelao on 5/30/23.
//

#ifndef SHELL_QUEUE_H
#define SHELL_QUEUE_H
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * Estructura que representa un nodo de la queue.
 * @param data El dato que contiene el nodo.
 * @param next El siguiente nodo.
 * */
typedef struct {
    void* data;
    struct Node *next;
} Node;

/**
 * Estructura que representa una queue.
 * @param head El primer nodo de la queue.
 * @param tail El ultimo nodo de la queue.
 * @param size El tamaño de la queue.
 * */
typedef struct {
    Node *head;
    Node *tail;
    int size;
} Queue;

Queue *create_queue();
void enqueue(Queue *queue, void *data, size_t type_size);
void *dequeue(Queue *queue);
void *dequeue_at(Queue *queue, int index);
void *get_at_queue(Queue *queue, int index);
void clear_queue(Queue *queue);
Node *create_node_queue(size_t type_size);
bool is_queue_empty(Queue *queue);
void print_queue(Queue *queue, void (*p_fun)(void*));
void *dequeue_node(Queue *queue, void *data, int (*p_cmp)(void*, void*));
bool contains(Queue *queue, void *data, int (*p_cmp)(void*, void*));
void sort_queue(Queue *queue, int (*p_cmp)(void*, void*));
#endif //SHELL_QUEUE_H
