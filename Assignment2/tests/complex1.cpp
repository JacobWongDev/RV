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
    Transition t1("q1", "q2", '1', &no_enable, 1);
    transitions.push_back(&t1);

    Transition t2("q2", "q3", '1', &no_enable, 3);
    transitions.push_back(&t2);

    Transition t3("q3", "q1", '1', &no_enable, 2);
    transitions.push_back(&t3);

    LowerBound en1(4, false);
    Transition t4("q2", "q4", '1', &en1, 3);
    transitions.push_back(&t4);

    UpperBound en2(10, false);
    Transition t5("q3", "q4", '1', &en2, 0);
    transitions.push_back(&t5);

    UpperBound en3(8, false);
    Transition t6("q3", "q5", '1', &en3, 0);
    transitions.push_back(&t6);

    Transition t7("q5", "q5", '1', &no_enable, 4);
    transitions.push_back(&t7);

    LowerBound en4(12, false);
    Transition t8("q5", "q6", '1', &en4, 0);
    transitions.push_back(&t8);

    LowerBound en5(23, false);
    Transition t9("q4", "q6", '1', &en5, 0);
    transitions.push_back(&t9);

    EFSM e(states, transitions, 0);
    auto run = e.reach("q1");
    print_run(run);
    delete run[0];

    // Try different starting point and initial value
    auto run2 = e.reach("q3");
    print_run(run2);
    delete run2[0];
    return 0;
}