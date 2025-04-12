/**
 * Using custom example.
 *
 * Want to check that a single state that is starting and accepting is permitted.
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
    State A("A", START | ACCEPTING);
    states.push_back(&A);
    EFSM e(states, transitions, e_transitions, vars);
    ASSERT("'<empty string>'", e.verify(""), true);
    return 0;
}