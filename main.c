#include "structures.h"
#include "system_utils.h"
#include "list_utils.h"
#include "operations.h"

int main() {
    FILE *in = fopen("tema1.in", "r");
    FILE *out = fopen("tema1.out", "w");

    /* Verify if files were successfully opened */
    if (!in || !out) {
        if (in) fclose(in);
        if (out) fclose(out);
    return 1;
}

    /* Read the total number of intervention units */
    int num_units;
    if (fscanf(in, "%d", &num_units) != 1) {
        fclose(in);
        fclose(out);
        return 1;
    }

    System *sys = init_system(num_units);
    if (!sys) {
        fclose(in);
        fclose(out);
        return 1;
    }

    /* Populate the units array and register them into the availability queue */
    for (int i = 0; i < num_units; i++) {
        fscanf(in, "%d %c", &sys->units[i].id, &sys->units[i].type);
        sys->units[i].availability = 1;

        enqueue(&(sys->queue_available_units), &sys->units[i]);
    }

    /* Read the total number of operations to execute */
    int num_ops;
    if (fscanf(in, "%d", &num_ops) != 1) {
        free_system(sys);
        fclose(in);
        fclose(out);
        return 1;
    }

    char command_buffer[50];
    for (int i = 0; i < num_ops; i++) {
        fscanf(in, "%s", command_buffer);

        /* Command dispatching will be wired to the operations module in upcoming commits */
        if (strcmp(command_buffer, "ADD_INCIDENT") == 0) {
            int id;
            char priority_buffer[10];
            char description_buffer[200];
            fscanf(in, "%d %s", &id, priority_buffer);
            fscanf(in, " \"%[^\"]\"", description_buffer);
            add_incident_operation(sys, id, priority_buffer, description_buffer);
        } else if (strcmp(command_buffer, "CHECK_UNITS_AVAILABILITY") == 0) {
            check_units_availability_operation(sys, out);
        } else if (strcmp(command_buffer, "DISPATCH") == 0) {
            /* Handled in operations module */
        } else if (strcmp(command_buffer, "UNDO_LAST_DISPATCH") == 0) {
            /* Handled in operations module */
        } else if (strcmp(command_buffer, "SOLVED_INCIDENT") == 0) {
            int id;
            fscanf(in, "%d", &id);
        } else if (strcmp(command_buffer, "SHOW_UNIT") == 0) {
            int id;
            fscanf(in, "%d", &id);
            show_unit_operation(sys, id, out);
        } else if (strcmp(command_buffer, "SHOW_INCIDENT") == 0) {
            int id;
            fscanf(in, "%d", &id);
            show_incident_operation(sys, id, out);
        } else if (strcmp(command_buffer, "SHOW_INTERVENTIONS") == 0) {
            /* Handled in operations module */
        }
    }

    free_system(sys);

    fclose(in);
    fclose(out);
    return 0;
}