/**
 * Monitoring the property
 * \varphi = \square p
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
    State q("q", START | ACCEPTING);
    states.push_back(&q);
    State q0("q0", DEFAULT);
    states.push_back(&q0);
    // Transitions
    LowerBound always_enabled(0, false);
    Transition t1("q", "q", 'p', &always_enabled, 0);
    transitions.push_back(&t1);
    Transition t2("q", "q0", 'q', &always_enabled, 0);
    transitions.push_back(&t2);
    Transition t3("q0", "q0", 'p', &always_enabled, 0);
    transitions.push_back(&t3);
    Transition t4("q0", "q0", 'q', &always_enabled, 0);
    transitions.push_back(&t4);

    EFSM e(states, transitions, alphabet, 0);

    Monitor monitor(&e);
    ASSERT_VERDICT("'ppppp'", monitor.monitor("ppppp"), UNKNOWN);
    ASSERT_VERDICT("'ppqpp'", monitor.monitor("ppqpp"), BOTTOM);
    ASSERT_VERDICT("'qppqpp'", monitor.monitor("qppqpp"), BOTTOM);
    ASSERT_VERDICT("'q'", monitor.monitor("q"), BOTTOM);
    return 0;
}