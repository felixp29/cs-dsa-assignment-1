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
