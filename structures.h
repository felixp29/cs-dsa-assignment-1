#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct unit {
    int id;
    char type;
    int availability;
} Unit;

typedef struct incident {
    int id;
    char priority[7];
    char *description;
    char status[11];
    struct incident *next;
    struct incident *prev;
} Incident, *IncidentList;

typedef struct intervention {
    struct incident *incident;
    struct unit *unit;
    struct intervention *next;
    struct intervention *prev;
} Intervention, *InterventionList;

typedef struct node {
    void *data;
    struct node *next;
} Node, *Stack;

typedef struct queue {
    Node *front;
    Node *back;
} Queue;

typedef struct system {
    Unit *units;
    int num_units;

    IncidentList incidents;
    InterventionList interventions;

    Queue *queue_high;
    Queue *queue_medium;
    Queue *queue_low;

    Queue *queue_available_units;

    Stack history_stack;
} System;

#endif