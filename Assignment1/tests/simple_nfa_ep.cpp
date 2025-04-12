/**
 * Using NFA from:
 *  Example 1.33 Pg. 52 in "Introduction to the Theory of Computation" 2nd. Edition by Sipser.
 */

#include "../efsm.hpp"
#include "../test.hpp"
/**
 * Main Function.
 */
int main() {
    std::vector<State*> states;
    std::vector<Transition*> transitions;
    std::vector<ETransition*> e_transitions;
    std::unordered_map<std::string, unsigned int> vars; // empty

    // Initialize states
    State q1("q1", START);
    states.push_back(&q1);
    State q2("q2", ACCEPTING);
    states.push_back(&q2);
    State q3("q3", ACCEPTING);
    states.push_back(&q3);
    State q4("q4", DEFAULT);
    states.push_back(&q4);
    State q5("q5", DEFAULT);
    states.push_back(&q5);
    State q6("q6", DEFAULT);
    states.push_back(&q6);

    // Transitions
    Transition t1("q2", {"q6"}, '0', TRUE, NOP);
    transitions.push_back(&t1);
    Transition t2("q6", {"q2"}, '0', TRUE, NOP);
    transitions.push_back(&t2);
    Transition t3("q3", {"q4"}, '0', TRUE, NOP);
    transitions.push_back(&t3);
    Transition t4("q4", {"q5"}, '0', TRUE, NOP);
    transitions.push_back(&t4);
    Transition t5("q5", {"q3"}, '0', TRUE, NOP);
    transitions.push_back(&t5);

    // epsilon transitions
    ETransition e1("q1", {"q2", "q3"});
    e_transitions.push_back(&e1);

    EFSM e(states, transitions, e_transitions, vars);

    ASSERT("'000'", e.verify("000"), true);
    ASSERT("'00'", e.verify("00"), true);
    ASSERT("'000000'", e.verify("000000"), true);
    ASSERT("'0'", e.verify("0"), false);
    ASSERT("'00000'", e.verify("00000"), false);
    return 0;
}