#include <iostream>

#define MINIFSM_IMPLEMENTATION
#include "minifsm.hpp"

void test_fsm() {
    using namespace minifsm;

    State start     = TemplatedStateModel<1, false>::make();
    State n_found   = TemplatedStateModel<2, false>::make();
    State i_found   = TemplatedStateModel<3, false>::make();
    State c_found   = TemplatedStateModel<4, false>::make();
    State success   = TemplatedStateModel<7, true>::make();


    Symbol n = TemplatedSymbolModel<1>::make();
    Symbol i = TemplatedSymbolModel<2>::make();
    Symbol c = TemplatedSymbolModel<3>::make();
    Symbol e = TemplatedSymbolModel<4>::make();

    Delta delta = {
            {start,   n, n_found},
            {n_found, i, i_found},
            {i_found, c, c_found},
            {c_found, e, success}
    };

    FSM fsm(start, delta);
    auto result = fsm.accepts({n, i, c, e});

}

int main() {
    test_fsm();
}