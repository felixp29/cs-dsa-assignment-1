#include "structures.h"
#include "list_utils.h"

void enqueue(Queue **q, void *data) {
    if (*q == NULL) {
        *q = malloc(sizeof(Queue));
        (*q)->front = (*q)->back = NULL;
    }
    Node *new_node = malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;
    if ((*q)->back == NULL) {
        (*q)->front = (*q)->back = new_node;
    } else {
        (*q)->back->next = new_node;
        (*q)->back = new_node;
    }
}

void* dequeue(Queue *q) {
    if (q == NULL || q->front == NULL) {
        return NULL;
    }
    Node *temp = q->front;
    void *data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->back = NULL;
    }
    free(temp);
    return data;
}

void push(Stack *top, void *data) {
    Node *new_node = malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = *top;
    *top = new_node;
}

void* pop(Stack *top) {
    if (*top == NULL) {
        return NULL;
    }
    Node *temp = *top;
    void *data = temp->data;
    *top = (*top)->next;
    free(temp);
    return data;
}