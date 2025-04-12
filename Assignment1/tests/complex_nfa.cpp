/**
 * Using custom example.
 *
 * See: docs/complex_nfa.jpg
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
    State A("A", START);
    states.push_back(&A);
    State B("B", START);
    states.push_back(&B);
    State C("C", DEFAULT);
    states.push_back(&C);
    State D("D", DEFAULT);
    states.push_back(&D);
    State E("E", DEFAULT);
    states.push_back(&E);
    State F("F", DEFAULT);
    states.push_back(&F);
    State G("G", DEFAULT);
    states.push_back(&G);
    State H("H", ACCEPTING);
    states.push_back(&H);

    // Transitions
    Transition t1("B", {"D"}, 'a', TRUE, NOP);
    transitions.push_back(&t1);
    Transition t2("D", {"C", "E", "F"}, 'b', TRUE, NOP);
    transitions.push_back(&t2);
    Transition t3("B", {"G"}, 'c', TRUE, NOP);
    transitions.push_back(&t3);
    Transition t4("E", {"H"}, 'z', TRUE, NOP);
    transitions.push_back(&t4);
    Transition t5("G", {"H"}, 'd', TRUE, NOP);
    transitions.push_back(&t5);

    // epsilon transitions
    struct ETransition e1("A", {"C"});
    e_transitions.push_back(&e1);
    struct ETransition e2("C", {"E"});
    e_transitions.push_back(&e2);
    struct ETransition e3("F", {"E", "H", "G"});
    e_transitions.push_back(&e3);

    EFSM e(states, transitions, e_transitions, vars);

    ASSERT("'cd'", e.verify("cd"), true);
    ASSERT("'abz'", e.verify("abz"), true);
    ASSERT("'z'", e.verify("z"), true);
    ASSERT("'ab'", e.verify("ab"), true);
    ASSERT("'abd'", e.verify("abd"), true);
    ASSERT("'c'", e.verify("c"), false);
    ASSERT("'d'", e.verify("d"), false);
    return 0;
}