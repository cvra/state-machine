#include "CppUTest/TestHarness.h"
#include <cstddef>

extern "C" {
#include "../state_machine.h"
}

void simple_state_1(state_machine_t *machine)
{

}

// General purpose call counter used in various tests
static int call_counter;


TEST_GROUP(StateMachineTestGroup)
{
    state_machine_t *machine;

    void setup(void)
    {
        machine = state_machine_create();
        call_counter = 0;
    }

    void teardown(void)
    {
        state_machine_delete(machine);
    }
};

TEST(StateMachineTestGroup, CanInitMachine)
{
    POINTERS_EQUAL(NULL, state_machine_get_current_state(machine));
}

TEST(StateMachineTestGroup, CanSetState)
{
    state_machine_set_current_state(machine, (state_func)0x1234);
    POINTERS_EQUAL(0x1234, state_machine_get_current_state(machine));
}

TEST(StateMachineTestGroup, EmptyStateExits)
{
    // In this test we want to check that running a machine from the NULL state
    // returns.
    // This test does not have any test macros, because the test app will crash
    // if it tries to execute the NULL pointer.

    state_machine_set_current_state(machine, NULL);
    state_machine_run(machine);
}

TEST(StateMachineTestGroup, NULLStateIsNotValid)
{
    state_machine_set_current_state(machine, NULL);
    CHECK_EQUAL(0, state_machine_state_is_valid(machine));

    state_machine_set_current_state(machine, simple_state_1);
    CHECK_EQUAL(1, state_machine_state_is_valid(machine));
}

void simple_exiting_state(state_machine_t *machine)
{
    state_machine_exit(machine);
    call_counter ++;
}

TEST(StateMachineTestGroup, CanExitStateMachine)
{
    state_machine_set_current_state(machine, simple_exiting_state);
    state_machine_run(machine);

    // Check that we really entered the state machine
    CHECK_EQUAL(1, call_counter);

    // At this point we should have exited the state machine and be in invalid
    // state
    CHECK_EQUAL(0, state_machine_state_is_valid(machine));
}


// Simple state machine made of two state that ping pong each other until they
// have been called 20 times
void chaining_state_2(state_machine_t *machine);
void chaining_state_1(state_machine_t *machine)
{
    call_counter ++;


    state_machine_set_current_state(machine, chaining_state_2);
}

void chaining_state_2(state_machine_t *machine)
{
    call_counter ++;

    if (call_counter >= 20) {
        state_machine_exit(machine);
        return;
    }

    state_machine_set_current_state(machine, chaining_state_1);
}

TEST(StateMachineTestGroup, CanTransitionBetweenStates)
{
    state_machine_set_current_state(machine, chaining_state_1);

    while (state_machine_state_is_valid(machine)) {
        state_machine_run(machine);
    }

    CHECK_EQUAL(20, call_counter);
}

TEST(StateMachineTestGroup, CanGetContext)
{
    POINTERS_EQUAL(NULL, state_machine_get_context(machine));
    state_machine_set_context(machine, (void *)1234);
    POINTERS_EQUAL(1234, state_machine_get_context(machine));
}

