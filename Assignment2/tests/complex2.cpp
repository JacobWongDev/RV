/**
 * Using custom example.
 *
 * See: docs/complex1.jpg
 */
#include "../efsm.hpp"
#include "../test.hpp"
/**
 * Main Function.
 */
int main() {
    std::vector<State*> states;
    std::vector<Transition*> transitions;
    // Initialize states
    State q1("q1", START);
    states.push_back(&q1);
    State q2("q2", DEFAULT);
    states.push_back(&q2);
    State q3("q3", ACCEPTING);
    states.push_back(&q3);
    State q4("q4", DEFAULT);
    states.push_back(&q4);

    // Transitions
    Interval intrvl1(0, true, 10, true);
    Transition t1("q1", "q2", '1', &intrvl1, 5);
    transitions.push_back(&t1);

    UpperBound upr1(1, true);
    Transition t2("q2", "q3", '1', &upr1, 0);
    transitions.push_back(&t2);

    LowerBound lwr1(10, true);
    Transition t3("q2", "q4", '1', &lwr1, 0);
    transitions.push_back(&t3);

    LowerBound no_enable(0, false);
    Transition t4("q4", "q4", '1', &no_enable, 1);
    transitions.push_back(&t4);

    LowerBound lwr2(20, true);
    Transition t5("q4", "q3", '1', &lwr2, 0);
    transitions.push_back(&t5);

    EFSM e(states, transitions, 0);
    auto run = e.reach("q1");
    print_run(run);
    delete run[0];
    return 0;
}