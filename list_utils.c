#include "structures.h"
#include "list_utils.h"

/* Search for an incident in the circular doubly linked list with sentinel.
 * Iterates through all registered incidents and returns a pointer
 * to the node matching the given ID, or NULL if not found. */
IncidentList find_incident_by_id(System *sys, int id) {
    IncidentList current = sys->incidents->next;
    while (current != sys->incidents) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/* Search for an intervention unit by ID in the units array.
 * Returns a pointer to the matching unit in the array, or NULL if not found. */
Unit *find_unit_by_id(System *sys, int id) {
    for (int i = 0; i < sys->num_units; i++) {
        if (sys->units[i].id == id) {
            return &sys->units[i];
        }
    }
    return NULL;
}

void add_incident_node(IncidentList dummy, IncidentList new_node) {
    IncidentList last = dummy->prev;

    new_node->next = dummy;
    new_node->prev = last;

    last->next = new_node;
    dummy->prev = new_node;
}

/* Worker function: frees all incident nodes and their allocated descriptions */
void free_incident(IncidentList dummy) {
    /* If sentinel does not exist, 
     * return immediately to prevent double freeing */
    if (dummy == NULL) {
        return;
    }

    IncidentList current = dummy->next;
    while (current != dummy) {
        /* Save the address of the next node before freeing the current one */
        IncidentList temp_next = current->next;
        if (current->description != NULL) {
            free(current->description);
        }
        free(current);
        current = temp_next;
    }

    /* Free sentinel description and the sentinel node itself */
    if (dummy->description != NULL) {
        free(dummy->description);
    }
    free(dummy);
}

/* Wrapper function: safely deallocates the incident list and
 * resets the pointer to NULL */
void free_incident_list_wrapper(IncidentList *dummy_ptr) {
    if (dummy_ptr == NULL || *dummy_ptr == NULL) {
        return;
    }
    free_incident(*dummy_ptr);
    *dummy_ptr = NULL;
}

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