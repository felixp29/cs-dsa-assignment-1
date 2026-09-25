#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>
#include "structures.h"

void add_incident_operation(System *sys, int id, char *priority, char *desc);
void show_incident_operation(System *sys, int id, FILE *out);
void show_unit_operation(System *sys, int id, FILE *out);
void check_units_availability_operation(System *sys, FILE *out);

#endif