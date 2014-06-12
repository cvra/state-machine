#include <stdlib.h>
#include "state_machine.h"

struct state_machine_s {
    void (*current_state)(state_machine_t *);

    /** The context of a state machine is some user-provided pointer which can
     * be used to store information about the state machine instance, such as
     * the current connection if the state machine is used to implement a server
     * for example. */
    void *context;
};

state_machine_t *state_machine_create(void)
{
    state_machine_t *machine;
    machine = malloc(sizeof(state_machine_t));

    state_machine_set_current_state(machine, NULL);
    state_machine_set_context(machine, NULL);

    return machine;
}

void state_machine_delete(state_machine_t *machine)
{
    free(machine);
}

void state_machine_set_current_state(state_machine_t *machine, state_func state)
{
    machine->current_state = state;
}

state_func state_machine_get_current_state(const state_machine_t *machine)
{
    return machine->current_state;
}

void state_machine_run(state_machine_t *machine)
{
    state_func current_state;

    if (state_machine_state_is_valid(machine)) {
        current_state = state_machine_get_current_state(machine);
        current_state(machine);
    }
}

void state_machine_exit(state_machine_t *machine)
{
    state_machine_set_current_state(machine, NULL);
}

int state_machine_state_is_valid(const state_machine_t *machine)
{
    state_func current_state;
    current_state = state_machine_get_current_state(machine);
    return current_state != NULL;
}

void state_machine_set_context(state_machine_t *machine, void *context)
{
    machine->context = context;
}

void *state_machine_get_context(const state_machine_t *machine)
{
    return machine->context;
}
