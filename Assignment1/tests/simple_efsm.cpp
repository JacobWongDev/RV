/**
 * Using custom example.
 *
 * See: docs/simple_efsm.jpg
 */

#include "../efsm.hpp"
#include "../test.hpp"

bool divisible_by_3(std::unordered_map<std::string, unsigned int>& vars) {
    if(vars["v"] % 3 == 0) {
        return true;
    } else {
        return false;
    }
}

void increment(std::unordered_map<std::string, unsigned int>& vars) {
    vars["v"]++;
}

/**
 * Main Function.
 */
int main() {
    std::vector<State*> states;
    std::vector<Transition*> transitions;
    std::vector<ETransition*> e_transitions;
    std::unordered_map<std::string, unsigned int> vars; // empty

    vars.insert(std::pair<std::string, unsigned int>("v", 0));

    // Initialize states
    State q1("q0", START);
    states.push_back(&q1);
    State q2("q1", DEFAULT);
    states.push_back(&q2);
    State q3("q2", ACCEPTING);
    states.push_back(&q3);

    // Transitions
    Transition t1("q0", {"q1"}, '2', TRUE, NOP);
    transitions.push_back(&t1);
    Transition t2("q1", {"q0"}, '3', TRUE, increment);
    transitions.push_back(&t2);
    Transition t3("q0", {"q2"}, '5', divisible_by_3, NOP);
    transitions.push_back(&t3);

    EFSM e(states, transitions, e_transitions, vars);

    ASSERT("'2323235'", e.verify("2323235"), true);
    ASSERT("'2323232323235'", e.verify("2323232323235"), true);
    ASSERT("'2323232323232323235'", e.verify("2323232323232323235"), true);
    ASSERT("'232325'", e.verify("232325"), false);
    ASSERT("'235'", e.verify("235"), false);
    return 0;
}