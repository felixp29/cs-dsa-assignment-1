#include <string.h>
#include "structures.h"
#include "list_utils.h"
#include "system_utils.h"

void add_incident_operation(System *sys, int id, char *priority, char *desc) {
    IncidentList new_inc = create_incident_node(id, priority, desc, "queued");
    add_incident_node(sys->incidents, new_inc);
    
    /* Enqueue incident according to priority level */
    if (strcmp(priority, "high") == 0) {
        enqueue(&(sys->queue_high), new_inc);
    } else if (strcmp(priority, "medium") == 0) {
        enqueue(&(sys->queue_medium), new_inc);
    } else if (strcmp(priority, "low") == 0) {
        enqueue(&(sys->queue_low), new_inc);
    }
}

void check_units_availability_operation(System *sys, FILE *out) {
    int count = 0;
    Node *current = sys->queue_available_units->front;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    fprintf(out, "Number of available units: %d\n", count);
}

void show_unit_operation(System *sys, int id, FILE *out) {
    Unit *u = find_unit_by_id(sys, id);
    if (u == NULL) {
        fprintf(out, "INVALID OPERATION! ERROR 404\n");
        return;
    }
    fprintf(out, "Unit %d is type %c and is %s\n", 
            u->id, u->type, u->availability ? "available" : "unavailable");
}

void show_incident_operation(System *sys, int id, FILE *out) {
    IncidentList inc = find_incident_by_id(sys, id);
    if (inc == NULL) {
        fprintf(out, "INVALID OPERATION! ERROR 404\n");
        return;
    }
    fprintf(out, "Incident %d has %s priority, the following description: \"%s\" and is %s\n",
            inc->id, inc->priority, inc->description, inc->status);
}

void dispatch_operation(System *sys, FILE *out) {
    Queue *q_target = NULL;

    /* Select the highest priority non-empty queue (high, medium, low) */
    if (sys->queue_high->front != NULL) {
        q_target = sys->queue_high;
    } else if (sys->queue_medium->front != NULL) {
        q_target = sys->queue_medium;
    } else if (sys->queue_low->front != NULL) {
        q_target = sys->queue_low;
    }

    if (q_target == NULL || sys->queue_available_units->front == NULL) {
        fprintf(out, "INVALID OPERATION! ERROR 404\n");
        return;
    }

    /* Dispatch the intervention: assign unit to incident and update records */
    IncidentList inc = (IncidentList)dequeue(q_target);
    Unit *u = (Unit *)dequeue(sys->queue_available_units);

    strcpy(inc->status, "intervened");
    u->availability = 0;

    InterventionList new_inter = malloc(sizeof(Intervention));
    new_inter->incident = inc;
    new_inter->unit = u;

    InterventionList last = sys->interventions->prev;
    new_inter->next = sys->interventions;
    new_inter->prev = last;
    last->next = new_inter;
    sys->interventions->prev = new_inter;

    push(&(sys->history_stack), new_inter);
}

void solved_incident_operation(System *sys, int id, FILE *out) {
    IncidentList inc = find_incident_by_id(sys, id);
    if (inc == NULL || strcmp(inc->status, "intervened") != 0) {
        fprintf(out, "INVALID OPERATION! ERROR 404\n");
        return;
    }

    /* Mark incident <id> as solved */
    strcpy(inc->status, "solved");

    /* Locate the assigned unit in the circular intervention list and release it */
    InterventionList current = sys->interventions->next;
    while (current != sys->interventions) {
        if (current->incident->id == id) {
            /* Release unit while keeping the intervention node in history stack */
            current->unit->availability = 1;
            enqueue(&(sys->queue_available_units), current->unit);
            return;
        }
        current = current->next;
    } 
}

void show_interventions_operation(System *sys, FILE *out) {
    InterventionList current = sys->interventions->next;
    if (current == sys->interventions) {
        fprintf(out, "No intervention has been initiated\n");
        return;
    }

    /* Print all active and solved interventions until sentinel is reached */
    while (current != sys->interventions) {
        fprintf(out, "Incident %d was assigned to unit %d, and has the following status: \"%s\"\n",
                current->incident->id, current->unit->id, current->incident->status);
        current = current->next;
    }
}

void undo_last_dispatch_operation(System *sys, FILE *out) {
    /* Search the history stack for the first unsolved intervention */
    while (sys->history_stack != NULL) {
        InterventionList inter = (InterventionList)sys->history_stack->data;

        /* Skip and discard interventions that are already marked as solved */
        if (strcmp(inter->incident->status, "solved") == 0) {
            pop(&(sys->history_stack));
            continue;
        }
        
        /* Revert incident status to queued and restore unit availability */
        strcpy(inter->incident->status, "queued");
        inter->unit->availability = 1;

        /* Reinsert the incident at the front of its designated priority queue */
        Queue *q_target = NULL;
        if (strcmp(inter->incident->priority, "high") == 0) {
            q_target = sys->queue_high;
        } else if (strcmp(inter->incident->priority, "medium") == 0) {
            q_target = sys->queue_medium;
        } else if (strcmp(inter->incident->priority, "low") == 0){
            q_target = sys->queue_low;
        }

        Node *new_node = malloc(sizeof(Node));
        new_node->data = inter->incident;
        new_node->next = q_target->front;
        q_target->front = new_node;

        /* If the queue was empty, the prepended node also becomes the back node */
        if (q_target->back == NULL) {
            q_target->back = new_node;
        }

        /* Return the unit to the available units queue */
        enqueue(&(sys->queue_available_units), inter->unit);

        /* Remove the intervention node from the circular list */
        inter->prev->next = inter->next;
        inter->next->prev = inter->prev;
        free(inter);

        /* Remove the entry from the top of the history stack */
        pop(&(sys->history_stack));
        return;
    }

    /* Output error if no active dispatch could be reverted */
    fprintf(out, "INVALID OPERATION! ERROR 404\n");
}
