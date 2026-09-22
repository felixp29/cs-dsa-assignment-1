#ifndef SYSTEM_UTILS_H
#define SYSTEM_UTILS_H

#include "structures.h"

IncidentList create_incident_node(int id, char* priority, char* desc, char* status);
System* init_system(int num_units_from_file);
void free_system(System *sys);

#endif