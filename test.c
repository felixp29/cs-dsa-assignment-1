#include <stdio.h>
#include <assert.h>
#include "structures.h"
#include "list_utils.h"

int main(void) {
    Stack s = NULL;
    int s_val1 = 10;
    int s_val2 = 20;

    push(&s, &s_val1);
    push(&s, &s_val2);

    assert(*(int *)pop(&s) == 20);
    assert(*(int *)pop(&s) == 10);
    assert(pop(&s) == NULL);
    printf("[PASS] Stack primitive tests passed.\n");

    Queue *q = NULL;
    int q_val1 = 100;
    int q_val2 = 200;

    enqueue(&q, &q_val1);
    enqueue(&q, &q_val2);

    assert(*(int *)dequeue(q) == 100);
    assert(*(int *)dequeue(q) == 200);
    assert(dequeue(q) == NULL);
    free(q);
    printf("[PASS] Queue primitive tests passed.\n");

    return 0;
}