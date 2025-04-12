/**
 * Using custom example.
 *
 * See: docs/simple2.jpg
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
    State q3("q3", DEFAULT);
    states.push_back(&q3);
    State q4("q4", DEFAULT);
    states.push_back(&q4);
    State q5("q5", ACCEPTING);
    states.push_back(&q5);

    // Transitions
    LowerBound en1(0, false);
    Transition t1("q1", "q4", '1', &en1, 1);
    transitions.push_back(&t1);
    Transition t2("q4", "q3", '1', &en1, 2);
    transitions.push_back(&t2);
    Transition t3("q3", "q2", '1', &en1, 4);
    transitions.push_back(&t3);
    Transition t4("q2", "q1", '1', &en1, 0);
    transitions.push_back(&t4);
    Transition t5("q4", "q2", '1', &en1, 4);
    transitions.push_back(&t5);
    Interval en2(4, true, 6, false);
    Transition t6("q4", "q5", '1', &en2, 0);
    transitions.push_back(&t6);
    EFSM e(states, transitions, 0);
    auto run = e.reach("q1");
    print_run(run);
    delete run[0];
    return 0;
}