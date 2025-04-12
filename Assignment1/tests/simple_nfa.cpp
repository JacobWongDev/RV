/**
 * Using NFA from:
 *  Example 1.30 Pg. 51 in "Introduction to the Theory of Computation" 2nd. Edition by Sipser.
 */

#include "../efsm.hpp"
#include "../test.hpp"
/**
 * Main Function.
 */
int main() {
    std::vector<State*> states;
    std::vector<Transition*> transitions;
    std::unordered_map<std::string, unsigned int> vars; // empty

    // No variables used

    // Initialize states
    State q1("q1", START);
    states.push_back(&q1);
    State q2("q2", DEFAULT);
    states.push_back(&q2);
    State q3("q3", DEFAULT);
    states.push_back(&q3);
    State q4("q4", ACCEPTING);
    states.push_back(&q4);

    // Transitions
    Transition t1("q1", {"q1", "q2"}, '1', TRUE, NOP);
    transitions.push_back(&t1);
    Transition t2("q1", {"q1"}, '0', TRUE, NOP);
    transitions.push_back(&t2);
    Transition t3("q2", {"q3"}, '1', TRUE, NOP);
    transitions.push_back(&t3);
    Transition t4("q2", {"q3"}, '0', TRUE, NOP);
    transitions.push_back(&t4);
    Transition t5("q3", {"q4"}, '0', TRUE, NOP);
    transitions.push_back(&t5);
    Transition t6("q3", {"q4"}, '1', TRUE, NOP);
    transitions.push_back(&t6);

    // No epsilon transitions
    std::vector<ETransition*> e_transitions;

    EFSM e(states, transitions, e_transitions, vars);

    ASSERT("'100'", e.verify("100"), true);
    ASSERT("'100101100'", e.verify("100101100"), true);
    ASSERT("'092318970120'", e.verify("11101010010101100"), true);
    ASSERT("'11'", e.verify("11"), false);
    ASSERT("'0000'", e.verify("0000"), false);
    return 0;
}