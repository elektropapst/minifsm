#include <iostream>

#define MINIFSM_IMPLEMENTATION
#include "minifsm.hpp"

void test_fsm() {
    using namespace minifsm;

    State eat   = SimpleState<0>::make();
    State sleep = FinalState<1>::make();
    State work  = SimpleState<2>::make();

    Symbol tired  = SimpleSymbol<0>::make();
    Symbol hungry = SimpleSymbol<1>::make();
    Symbol bored  = SimpleSymbol<2>::make();

    Delta delta = {
            {eat, tired,  sleep},
            {eat, hungry, eat},
            {eat, bored,  work},

            {sleep, tired,  sleep},
            {sleep, hungry, eat},
            {sleep, bored,  work},

            {work, tired,  sleep},
            {work, hungry, eat},
            {work, bored,  work}
    };

    FSM stateMachine(sleep, delta);
    stateMachine.transit({tired, hungry, bored, tired});

}

int main() {
    test_fsm();
}