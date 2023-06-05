//
// Created by yaelao on 5/30/23.
//

#include "Queue.h"
/**
 * Crea una queue.
 * @return Una queue vacia.
 * */
Queue *create_queue() {
    Queue *queue = (Queue *) malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}

/**
 * Crea un nodo de la queue.
 * @param type_size El tamaño del tipo de dato que contendra el nodo.
 * @return Un nodo vacio.
 * */

Node *create_node_queue(size_t type_size){
    Node *node = (Node *) malloc(sizeof(Node));
    node->data = malloc(type_size);
    node->next = NULL;
    return node;
}

/**
 * Remueve un nodo de la queue en la posicion indicada.
 * @param queue La queue de donde se remueve el nodo.
 * @param index La posicion del nodo a remover.
 * @return El dato del nodo removido.
 * */
void *dequeue_at(Queue *queue, int index){
    Node *current = queue->head;
    Node *previous = NULL;
    void *data_removed = NULL;
    int i;

    if (index == 0) {
        data_removed = current->data;
        queue->head = (Node *) current->next;
        free(current);
        queue->size--;
        return data_removed;
    }

    for (i = 0; i < index; i++) {
        previous = current;
        current = (Node *) current->next;
    }

    data_removed = current->data;
    previous->next = current->next;
    free(current);
    queue->size--;

    return data_removed;
}

/**
 * Agrega un nodo a la queue.
 * @param queue La queue donde se agregara.
 * @param data El dato que contendra el nodo.
 * @param type_size El tamaño del tipo de dato que contendra el nodo.
 * */
void enqueue(Queue *queue, void *data, size_t type_size) {
    Node *node = create_node_queue(type_size);
    memcpy(node->data, data, type_size);
    if (is_queue_empty(queue)) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = (struct Node *) node;
        queue->tail = node;
    }
    queue->size++;
}

/**
 * Remueve un nodo de la queue siguiendo el principio FIFO.
 * @param queue La queue de donde se remueve el nodo.
 * @return El dato del nodo removido.
 * */
void *dequeue(Queue *queue) {
    if (is_queue_empty(queue)) {
        return NULL;
    }
    Node *node = queue->head;
    void *data = node->data;
    queue->head = (Node *) node->next;
    free(node);
    queue->size--;
    return data;
}

/**
 * Remeve el nodo inicado de la queue.
 * @param queue La queue de donde se remueve el nodo.
 * @param data El dato del nodo a remover.
 * @param p_cmp La funcion que compara el dato del nodo con el dato indicado.
 * @return El dato del nodo removido.
 * */
void *dequeue_node(Queue *queue, void *data, int (*p_cmp)(void*, void*)){
    Node *current = queue->head;
    Node *previous = NULL;
    void *data_removed = NULL;

    while (current != NULL) {
        if (p_cmp(current->data, data) == 0) {
            if (previous == NULL) {
                data_removed = current->data;
                queue->head = (Node *) current->next;
                free(current);
                queue->size--;
                return data_removed;
            }
            data_removed = current->data;
            previous->next = current->next;
            free(current);
            queue->size--;
            return data_removed;
        }
        previous = current;
        current = (Node *) current->next;
    }

    return NULL;
}

/**
 * Remeve todos los nodos de la queue.
 * */
void clear_queue(Queue *queue) {
    while (!is_queue_empty(queue)) {
        free(dequeue(queue));
    }
}

/**
 * Verifica si la queue esta vacia.
 * */
bool is_queue_empty(Queue *queue) {
    return queue->size == 0;
}

/**
 * Imprime el contentenido de la queue.
 * @param queue La queue a imprimir.
 * @param p_fun La funcion que imprime el dato del nodo.
 * */
void print_queue(Queue *queue, void (*p_fun)(void*)) {
    Node *node = queue->head;
    if (node == NULL) {
        printf("Process queue is empty.\n");
        return;
    }

    while (node != NULL) {
        p_fun(node->data);
        node = (Node *) node->next;
    }
}

bool contains(Queue *queue, void *data, int (*p_cmp)(void*, void*)){
    Node *node = queue->head;
    if (node == NULL) {
        return false;
    }

    while (node != NULL) {
        if (p_cmp(node->data, data) == 0) {
            return true;
        }
        node = (Node *) node->next;
    }

    return false;
}

/**
 * Ordena la queue.
 * @param queue La queue a ordenar.
 * @param p_cmp La función que compara el dato del nodo con el dato indicado.
 *
*/

void sort_queue(Queue *queue, int (*p_cmp)(void*, void*)){
    Node *current = queue->head;
    Node *index = NULL;
    void *temp = NULL;

    if (queue->head == NULL) {
        return;
    } else {
        while (current != NULL) {
            index = (Node *) current->next;

            while (index != NULL) {
                if (p_cmp(current->data, index->data) > 0) {
                    temp = current->data;
                    current->data = index->data;
                    index->data = temp;
                }
                index = (Node *) index->next;
            }
            current = (Node *) current->next;
        }
    }
}