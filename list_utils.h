#ifndef LIST_UTILS_H
#define LIST_UTILS_H

#include "structures.h"

IncidentList find_incident_by_id(System *sys, int id);
Unit* find_unit_by_id(System *sys, int id);
void free_incident(IncidentList dummy);
void free_incident_list_wrapper(IncidentList *sentinel_ptr);
void add_incident_node(IncidentList dummy, IncidentList new_node);
void enqueue(Queue **q, void *data);
void *dequeue(Queue *q);
void push(Stack *top, void *data);
void *pop(Stack *top);

#endif