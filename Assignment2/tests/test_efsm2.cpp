/**
 * Using custom example.
 *
 * See: docs/test_efsm2.jpg
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
    State q5("q5", DEFAULT);
    states.push_back(&q5);
    State q6("q6", ACCEPTING);
    states.push_back(&q6);

    // Transitions
    LowerBound no_enable(0, false);
    Transition t1("q1", "q2", 'b', &no_enable, 1);
    transitions.push_back(&t1);

    Transition t2("q2", "q3", 'a', &no_enable, 3);
    transitions.push_back(&t2);

    Transition t3("q3", "q1", 'a', &no_enable, 2);
    transitions.push_back(&t3);

    LowerBound en1(4, false);
    Transition t4("q2", "q4", 'c', &en1, 3);
    transitions.push_back(&t4);

    UpperBound en2(10, false);
    Transition t5("q3", "q4", 'c', &en2, 0);
    transitions.push_back(&t5);

    UpperBound en3(8, false);
    Transition t6("q3", "q5", 'd', &en3, 0);
    transitions.push_back(&t6);

    Transition t7("q5", "q5", 'a', &no_enable, 4);
    transitions.push_back(&t7);

    LowerBound en4(12, false);
    Transition t8("q5", "q6", 'f', &en4, 0);
    transitions.push_back(&t8);

    LowerBound en5(23, false);
    Transition t9("q4", "q6", 'g', &en5, 0);
    transitions.push_back(&t9);

    EFSM e(states, transitions, 0);
    ASSERT("'badaaaaf'", e.verify("badaaaaf"), true);
    ASSERT("'fabc'", e.verify("fabc"), false);
    ASSERT("'ccdf'", e.verify("ccdf"), false);
    return 0;
}