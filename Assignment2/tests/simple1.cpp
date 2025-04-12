/**
 * Using custom example.
 *
 * See: docs/simple1.jpg
 *
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

    // Transitions
    LowerBound en1(0, false);
    Transition t1("q1", "q1", '1', &en1, 2);
    transitions.push_back(&t1);
    LowerBound en2(0, false);
    Transition t2("q1", "q2", '1', &en2, 0);
    transitions.push_back(&t2);
    LowerBound en3(0, false);
    Transition t3("q2", "q2", '1', &en3, 3);
    transitions.push_back(&t3);
    Interval en4(10, false, 11, true);
    Transition t4("q2", "q3", '1', &en4, 0);
    transitions.push_back(&t4);
    EFSM e(states, transitions, 0);
    auto run = e.reach("q1");
    print_run(run);
    delete run[0];
    return 0;
}