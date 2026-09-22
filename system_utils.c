#include "structures.h"
#include "list_utils.h"

IncidentList create_incident_node(int id, char* priority, char* desc, char* status) {
    IncidentList new_node = malloc(sizeof(Incident));
    if (!new_node)
        return NULL;

    new_node->id = id;
    strcpy(new_node->priority, priority);

    new_node->description = malloc(strlen(desc) + 1);
    if (new_node->description) {
        strcpy(new_node->description, desc);
    }

    strcpy(new_node->status, status);

    return new_node;
}

System* init_system(int num_units_from_file) {
    System* sys = malloc(sizeof(System));
    if (!sys) {
        return NULL;
    }

    /* Dynamically allocate the units array */
    sys->units = malloc(num_units_from_file * sizeof(Unit));
    sys->num_units = num_units_from_file;

    /* Sentinel node for circular incidents list */
    sys->incidents = create_incident_node(0, "low", "test incident", "solved");
    sys->incidents->next = sys->incidents;
    sys->incidents->prev = sys->incidents;

    /* Sentinel node for circular interventions list */
    sys->interventions = malloc(sizeof(Intervention));
    sys->interventions->incident = NULL;
    sys->interventions->unit = NULL;
    sys->interventions->next = sys->interventions;
    sys->interventions->prev = sys->interventions;

    /* Initialize queues and history stack */
    sys->queue_high = calloc(1, sizeof(Queue));
    sys->queue_medium = calloc(1, sizeof(Queue));
    sys->queue_low = calloc(1, sizeof(Queue));
    sys->queue_available_units = calloc(1, sizeof(Queue));
    sys->history_stack = NULL;

    return sys;
}

void free_system(System *sys) {
    if (!sys) {
        return;
    }

    /* 1. Incidents
     * Wrapper calls free_incident() to release nodes and
     * descriptions, then resets pointer to NULL */
    free_incident_list_wrapper(&(sys->incidents));

    /* 2. History stack
     * pop() handles freeing internal node memory */
    while (sys->history_stack != NULL) {
        pop(&(sys->history_stack));
    }

    /* 3. Remaining interventions in the circular list */
    InterventionList current = sys->interventions->next;
    while (current != sys->interventions) {
        InterventionList next = current->next;
        free(current);
        current = next;
    }
    free(sys->interventions);

    /* 4. Queues
     * Iterate through all 4 queues to deallocate pending nodes and the queue headers */
    Queue* queues[] = {sys->queue_high, sys->queue_medium, sys->queue_low, sys->queue_available_units};
    for (int i = 0; i < 4; i++) {
        if (queues[i]) {
            while (queues[i]->front != NULL) {
                Node *tmp = queues[i]->front;
                queues[i]->front = queues[i]->front->next;
                free(tmp);
            }
            free(queues[i]);
        }
    }

    /* 5. Units array */
    free(sys->units);

    free(sys);
}