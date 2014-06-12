# State machine module
[![Build Status](https://travis-ci.org/cvra/state-machine.png)](https://travis-ci.org/cvra/state-machine)

This modules provides helper function to implement Finite State Machines
(http://en.wikipedia.org/wiki/Finite-state_machine) in C.
The states are implemented as function pointers which scale much better than
implementations using `switch/case` construction.
It also allows the compiler to check if a state is valid, which is not possible
using enums or defines for states.


## States
States are implemented as a function that will be called in loop as long as the
machine stays in the same state.

## Transitions
Transition from one state to another is simply handled by calling
`state_machine_set_current_state` with the destination state as a parameter,
then returning from the state function.

## Example
Simple example of a state machine that can be used to match C++ style ("//")
comments on text read from stdin.

```cpp
#include <stdio.h>
#include "state_machine.h"

void state1(state_machine_t *machine);
void state2(state_machine_t *machine);

void main(void)
{
    state_machine_t *machine;
    machine = state_machine_create();
    state_machine_set_current_state(machine, state1);

    while (state_machine_state_is_valid(machine)) {
        state_machine_run(machine);
    }

    state_machine_delete(machine);
}

void state1(state_machine_t *machine)
{
    /* If we read '/' change state to state2, otherwise stay in state1. */
    if (getchar() == '/') {
        state_machine_set_current_state(machine, state2);
    }
}

void state2(state_machine_t *machine)
{
    if (getchar() == '/') {
        state_machine_exit(machine);
    } else {
        state_machine_set_current_state(machine, state1);
    }
}
```

## Sanity checks
For the moment no sanity checking is performed, which means you can transition
to a state made for another state machine.

## Dependencies
None

