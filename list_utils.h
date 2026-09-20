#ifndef LIST_UTILS_H
#define LIST_UTILS_H

#include "structures.h"

void enqueue(Queue **q, void *data);
void *dequeue(Queue *q);
void push(Stack *top, void *data);
void *pop(Stack *top);

#endif