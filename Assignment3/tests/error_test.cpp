/**
 * Program to test error handling of interface.
 */

#include "../monitor.hpp"
#include "../test.hpp"

/**
 * Main Function.
 */
int main() {
    std::vector<State*> states;
    std::vector<Transition*> transitions;
    std::vector<char> alphabet = {'a'};

    /**
     * No states test
     */
    std::cout << "Try to instantiate with no states" << std::endl;
    try {
        EFSM e(states, transitions, alphabet, 0);
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }

    /**
     * Empty alphabet test
     */
    std::vector<char> alphabet2;
    State A0("A", START | ACCEPTING);
    states.push_back(&A0);
    std::cout << "Try to instantiate with empty alphabet" << std::endl;
    try {
        EFSM e(states, transitions, alphabet2, 0);
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
        EFSM e(states, transitions, alphabet, 0);
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    states.clear();

    std::cout << "Try to use state that is START and ACCEPTING (should be NO exception)" << std::endl;
    State A1("A", START | ACCEPTING);
    states.push_back(&A1);
    try {
        EFSM e(states, transitions, alphabet, 0);
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    states.clear();

    std::cout << "Try to use state that is DEFAULT (should be NO exception)" << std::endl;
    State A2("A", DEFAULT);
    State B("B", START | ACCEPTING);
    states.push_back(&A2);
    states.push_back(&B);
    try {
        EFSM e(states, transitions, alphabet, 0);
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    states.clear();

    std::cout << "Try to use state that is 0" << std::endl;
    State A3("A", 0);
    states.push_back(&A3);
    try {
        EFSM e(states, transitions, alphabet, 0);
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    states.clear();

    std::cout << "Try to use state that is 100" << std::endl;
    State A4("A", 100);
    states.push_back(&A4);
    try {
        EFSM e(states, transitions, alphabet, 0);
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    states.clear();

    std::cout << "Define with no start state" << std::endl;
    State A5("A", DEFAULT);
    states.push_back(&A5);
    try {
        EFSM e(states, transitions, alphabet, 0);
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    states.clear();

    std::cout << "Try to start with no states" << std::endl;
    try {
        EFSM e(states, transitions, alphabet, 0);
    } catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }

    states.clear();
    std::cout << "Try to use state not defined using state objects" << std::endl;
    State A6("a", START | ACCEPTING);
    states.push_back(&A6);
    LowerBound lb(0, false);
    Transition t = {"b", "c", '!', &lb, 0};
    transitions.push_back(&t);
    try {
        EFSM e(states, transitions, alphabet, 0);
    } catch(const std::out_of_range& e) {
        std::cout << "ERROR: " << e.what() << std::endl;
    }
    return 0;
}