//
// Created by yaelao on 5/30/23.
//

#include "List.h"

/**
 * Agrega un nodo al final de la lista.
 * @param list La lista a la que se le agregara el nodo.
 * @param data El dato que contendra el nodo.
 * @param type_size El tamaño del tipo de dato que contendra el nodo.
 * */
void append(List *list, void *data, size_t type_size) {
    Node *newNode = create_node_list(type_size);
    memcpy(newNode->data, data, type_size);

    if (is_list_empty(list) == true) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = (struct Node *) newNode;
        list->tail = newNode;
    }
    list->size++;
}

/**
 * Agrega un nodo en la posicion indicada.
 * @param list La lista a la que se le agregara el nodo.
 * @param data El dato que contendra el nodo.
 * @param index La posicion en la que se agregara el nodo.
 * @param type_size El tamaño del tipo de dato que contendra el nodo.
 * */
void add_at(List *list, void *data, int index, size_t type_size) {
    if (index < 0 || index > list->size) {
        perror("Index out of bounds");
        return;
    }

    Node *newNode = create_node_list(type_size);
    memcpy(newNode->data, data, type_size);

    if (index == 0) {
        newNode->next = (struct Node *) list->head;
        list->head = newNode;
    } else if (index == list->size) {
        list->tail->next = (struct Node *) newNode;
        list->tail = newNode;
    } else {
        Node *current = list->head;
        int i = 0;
        while (i++ < index - 1) {
            current = (Node *) current->next;
        }
        newNode->next = current->next;
        current->next = (struct Node *) newNode;
    }
    list->size++;
}

/**
 * Comprueba si la lista esta vacia.
 * @param list La lista a comprobar.
 * @return true si la lista esta vacia, false en caso contrario.
 * */
bool is_list_empty(List *list) {
    return list->size == 0;
}

/**
 * Remueve un nodo de la lista.
 * @param list La lista de la que se remueve el nodo.
 * @param data El dato que se usara para comparar con los nodos de la lista.
 * @param cmp_func La funcion de comparacion.
 * @return true si el nodo fue removido, false en caso contrario.
 *
 * */
void *remove_node_list(List *list, void *data, int (*cmp_func)(void *, void *)) {
    Node *current = list->head;
    Node *previous = NULL;

    while (current != NULL) {
        if (cmp_func(current->data, data) == 0) {
            if (previous == NULL) {
                list->head = (Node *) current->next;
            } else {
                previous->next = current->next;
            }
            if (current == list->tail) {
                list->tail = previous;
            }
            list->size--;
            void *temp = current->data;
            free(current);
            return temp;
        }
        previous = current;
        current = (Node *) current->next;
    }
    return NULL;
}

/**
 * Remueve un nodo de la lista.
 * @param list La lista de la que se remueve el nodo.
 * @param index El indice del nodo a remover.
 * @return El dato del nodo removido.
 * */
void *remove_element_at(List *list, int index) {
    if (index < 0 || index >= list->size) {
        perror("Index out of bounds");
        return NULL;
    }

    Node *current = list->head;
    Node *previous = NULL;

    for (int i = 0; i < index; i++) {
        previous = current;
        current = (Node *) current->next;
    }

    if (previous == NULL) {
        list->head = (Node *) current->next;
    } else {
        previous->next = current->next;
    }
    if (current == list->tail) {
        list->tail = previous;
    }
    list->size--;
    void *temp = current->data;
    free(current);
    return temp;
}

/**
 * Obtiene el dato de un nodo de la lista.
 * @param list La lista de la que se obtiene el dato.
 * @param index El indice del nodo.
 * @return El dato del nodo.
 * */
void *get_at(List *list, int index) {
    if (index < 0 || index >= list->size) {
        perror("Index out of bounds");
        return NULL;
    }

    Node *current = list->head;

    for (int i = 0; i < index; i++) {
        current = (Node *) current->next;
    }

    return current->data;
}

/**
 * Retorna el indice de un elemento de la lista.
 * @param list La lista en la que se buscara el elemento.
 * @param data El dato que se usara para comparar con los nodos de la lista.
 * @param cmp_func La funcion de comparacion.
 * @return El indice del elemento, -1 si no se encuentra.
 * */

int index_of(List *list, void *data, int (*cmp_func)(void *, void *)) {
    Node *current = list->head;
    int index = 0;

    while (current != NULL) {
        if (cmp_func(current->data, data) == 0) {
            return index;
        }
        current = (Node *) current->next;
        index++;
    }
    return -1;
}

/**
 * Ayuda a crear un nodo.
 * @param type_size El tamaño del tipo de dato que contendra el nodo.
 * @return El nodo creado.
 * */
Node *create_node_list(size_t type_size) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    newNode->data = malloc(type_size);
    newNode->next = NULL;
    return newNode;
}

/**
 * Crea una lista, inicializando sus atributos y reservando memoria para la misma.
 * */
List *create_list() {
    List *list = (List *) malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

/**
 * Imprime la lista.
 * @param list La lista a imprimir.
 * @param p_fun La funcion que se usara para imprimir el dato de cada nodo.
 * */
void print_list(List *list, void (*p_fun)(void*)) {
    Node *current = list->head;
    int i = 1;
    while (current != NULL) {
        printf("\t\n#%d \n", i++);
        p_fun(current->data);
        current = (Node *) current->next;
    }
}

/**
 * Libera la memoria reservada para la lista.
 * @param list La lista a liberar.
 * */
void clear_list(List *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *temp = current;
        current = (Node *) current->next;
        free(temp->data);
        free(temp);
    }
    free(list);
}

void *get_at_queue(Queue *queue, int index) {
    if (index < 0 || index >= queue->size) {
        perror("Index out of bounds");
        return NULL;
    }

    Node *current = queue->head;

    for (int i = 0; i < index; i++) {
        current = (Node *) current->next;
    }

    return current->data;
}
