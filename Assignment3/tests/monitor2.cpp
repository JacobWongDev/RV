/**
 * Monitoring the property
 * \varphi = \diamond p
 */
#include "../monitor.hpp"
#include "../test.hpp"
 /**
  * Main Function.
  */
int main() {
    std::vector<State*> states;
    std::vector<Transition*> transitions;
    std::vector<char> alphabet = {'p', 'q'};

    // Initialize states
    State q("q", START);
    states.push_back(&q);
    State q0("q0", ACCEPTING);
    states.push_back(&q0);
    // Transitions
    LowerBound always_enabled(0, false);
    Transition t1("q", "q", 'q', &always_enabled, 0);
    transitions.push_back(&t1);
    Transition t2("q", "q0", 'p', &always_enabled, 0);
    transitions.push_back(&t2);
    Transition t3("q0", "q0", 'p', &always_enabled, 0);
    transitions.push_back(&t3);
    Transition t4("q0", "q0", 'q', &always_enabled, 0);
    transitions.push_back(&t4);

    EFSM e(states, transitions, alphabet, 0);

    Monitor monitor(&e);
    ASSERT_VERDICT("'p'", monitor.monitor("p"), TOP);
    ASSERT_VERDICT("'qqqqqqp'", monitor.monitor("ppqpp"), TOP);
    ASSERT_VERDICT("'qqqqq'", monitor.monitor("qqqqq"), UNKNOWN);
    return 0;
}