## Emergency Dispatch Simulation System (112)

Simulation of a 112 emergency dispatch system implemented in C for the Data Structures & Algorithms course.

---

### Project Structure

* `structures.h` - defines the data structures requested by the assignment requirements along with internal helper wrappers:

  * `Unit`: dynamically allocated array storing available and deployed units

  * `Incident`: circular doubly linked list with a dummy sentinel node
  
  * `Intervention`: circular doubly linked list with sentinel storing pointers to active incidents and their assigned units
  
  * `Node` / `Stack`: singly linked list used as a LIFO stack to keep the history of dispatches for the undo operation
  
  * `Queue`: a structure with `front` and `back` pointers managing a singly linked list for FIFO operations
  
  * `System`: encapsulates all resources (units array, lists, queues, history stack) into a single pointer to avoid global variables


* `list_utils.h` & `list_utils.c` - core data structure logic:

  * stacks & queues: `push` / `pop` and `enqueue` / `dequeue` using double pointers (`**top`, `**q`) for in-place pointer updates

  * search helpers: node lookup functions like (`find_incident_by_id`, `find_unit_by_id`)

  * cleanup: `free_incident_list_wrapper` deallocates memory and sets the pointer to `NULL` to prevent dangling pointers


* `system_utils.h` & `system_utils.c` - system setup and cleanup:

  * `init_system()`: allocates the `System` structure, the units array, queues, and list sentinels

  * `create_incident_node()`: allocates a new incident node and exact memory for its description (`strlen(desc) + 1`)

  * `free_system()`: frees all allocated resources (nodes, strings, queues, and the system struct) to prevent memory leaks


* `operations.h` & `operations.c` - implements the required dispatch protocol:

  * `ADD_INCIDENT`: enqueues incoming incidents into priority-specific queues (`queue_high`, `queue_medium`, `queue_low`)

  * `CHECK_UNITS_AVAILABILITY`: prints the number of available units by checking the size of `queue_available_units`

  * `DISPATCH`: dequeues the highest priority incident, pairs it with the first available unit, appends the intervention node, and pushes the event to the history stack

  * `UNDO_LAST_DISPATCH`: searches the history stack for the latest unsolved intervention, resets the incident to `queued`, and releases the unit

  * `SOLVED_INCIDENT`: marks active interventions as solved and returns the unit to `queue_available_units` while retaining the node in history

  * `SHOW_UNIT` / `SHOW_INCIDENT` / `SHOW_INTERVENTIONS`: display information about units, incidents, and initiated interventions

* `main.c`: handles file I/O (`tema1.in` and `tema1.out`), parses commands in a loop with `strcmp`, and calls the corresponding operations

