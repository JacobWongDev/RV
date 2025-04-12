/**
 * Using custom example.
 *
 * See: docs/complex_efsm.jpg
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

void increment_v(std::unordered_map<std::string, unsigned int>& vars) {
    vars["v"]++;
}

void increment_n(std::unordered_map<std::string, unsigned int>& vars) {
    vars["n"] += 2;
}

bool end_condition(std::unordered_map<std::string, unsigned int>& vars) {
    if(vars["v"] == 4 && vars["n"] % 2 == 0) return true;
    else return false;
}

/**
 * Main Function.
 */
int main() {
    std::vector<State*> states;
    std::vector<Transition*> transitions;
    std::vector<ETransition*> e_transitions;
    std::unordered_map<std::string, unsigned int> vars;
    vars.insert(std::pair<std::string, unsigned int>("v", 0));
    vars.insert(std::pair<std::string, unsigned int>("n", 0));

    // Initialize states
    State A("A", START);
    states.push_back(&A);
    State B("B", DEFAULT);
    states.push_back(&B);
    State C("C", DEFAULT);
    states.push_back(&C);
    State D("D", ACCEPTING);
    states.push_back(&D);

    // Transitions
    Transition t1("A", {"B"}, 'a', TRUE, increment_v);
    transitions.push_back(&t1);
    Transition t3("B", {"C"}, 'a', TRUE, increment_v);
    transitions.push_back(&t3);
    Transition t6("C", {"A"}, 'a', TRUE, increment_n);
    transitions.push_back(&t6);
    Transition t7("C", {"D"}, 'c', end_condition, NOP);
    transitions.push_back(&t7);


    ETransition e1("A", {"D"});
    e_transitions.push_back(&e1);

    EFSM e(states, transitions, e_transitions, vars);

    ASSERT("'aaaaac'", e.verify("aaaaac"), true);
    ASSERT("'<empty string>'", e.verify(""), true);
    return 0;
}