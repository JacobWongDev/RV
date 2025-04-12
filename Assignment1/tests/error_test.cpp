/**
 * Program to test error handling of interface.
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

    /**
     * No states test
     */
    std::cout << "Try to instantiate with no states" << std::endl;
    try {
        EFSM e(states, transitions, e_transitions, vars);
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }

    /**
     * Invalid states test
     */
    std::cout << "Try to use state that is DEFAULT and START or ACCEPTING" << std::endl;
    State A("A", START | ACCEPTING | DEFAULT);
    states.push_back(&A);
    try {
        EFSM e(states, transitions, e_transitions, vars);
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    states.clear();

    std::cout << "Try to use state that is START and ACCEPTING (should be NO exception)" << std::endl;
    State B("A", START | ACCEPTING);
    states.push_back(&B);
    try {
        EFSM e(states, transitions, e_transitions, vars);
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    states.clear();

    std::cout << "Try to use state that is DEFAULT (should be NO exception)" << std::endl;
    State C("A", DEFAULT);
    State D("B", START | ACCEPTING);
    states.push_back(&C);
    states.push_back(&D);
    try {
        EFSM e(states, transitions, e_transitions, vars);
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    states.clear();

    std::cout << "Try to use state that is 0" << std::endl;
    State E("A", 0);
    states.push_back(&E);
    try {
        EFSM e(states, transitions, e_transitions, vars);
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    states.clear();

    std::cout << "Try to use state that is 100" << std::endl;
    State F("A", 100);
    states.push_back(&F);
    try {
        EFSM e(states, transitions, e_transitions, vars);
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    states.clear();

    /**
     * Epsilon loop test
     */
    State G("A", START);
    states.push_back(&G);
    State H("B", DEFAULT);
    states.push_back(&H);
    State I("C", DEFAULT);
    states.push_back(&I);
    State J("D", ACCEPTING);
    states.push_back(&J);

    ETransition e1("A", {"B"});
    e_transitions.push_back(&e1);
    ETransition e2("B", {"C"});
    e_transitions.push_back(&e2);
    ETransition e3("C", {"D"});
    e_transitions.push_back(&e3);
    ETransition e4("D", {"A"});
    e_transitions.push_back(&e4);

    std::cout << "Try to make epsilon loop" << std::endl;
    try {
        EFSM e(states, transitions, e_transitions, vars);
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << "Try to use state not defined using state objects" << std::endl;
    ETransition e5("Z", {"B"});
    e_transitions.push_back(&e5);
    try {
        EFSM e(states, transitions, e_transitions, vars);
    } catch(const std::out_of_range& e) {
        std::cout << "ERROR: " << e.what() << std::endl;
    }
    return 0;
}