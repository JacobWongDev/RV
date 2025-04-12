/**
 * Using custom example.
 *
 * The language for this input FSM is
 * b^5a(ab)^*
 *
 * See: docs/monitor5.jpg
 */
#include "../monitor.hpp"
#include "../test.hpp"
 /**
  * Main Function.
  */
int main() {
    std::vector<State*> states;
    std::vector<Transition*> transitions;
    std::vector<char> alphabet = {
        'a',
        'b',
    };

    // Initialize states
    State q0("q0", START);
    states.push_back(&q0);
    State q1("q1", ACCEPTING);
    states.push_back(&q1);
    // Transitions
    LowerBound always_enabled(0, false);
    Transition t1("q0", "q0", 'b', &always_enabled, 1);
    transitions.push_back(&t1);
    Interval five(5, false, 5, false);
    Transition t2("q0", "q1", 'a', &five, 0);
    transitions.push_back(&t2);
    Transition t3("q1", "q1", 'a', &always_enabled, 0);
    transitions.push_back(&t3);
    Transition t4("q1", "q1", 'b', &always_enabled, 0);
    transitions.push_back(&t4);
    EFSM e(states, transitions, alphabet, 0);

    Monitor monitor(&e);
    ASSERT_VERDICT("'a'", monitor.monitor("a"), BOTTOM);
    ASSERT_VERDICT("'aaaaaaaaa'", monitor.monitor("aaaaaaaaa"), BOTTOM);
    ASSERT_VERDICT("'aabbbbbaaaaaaa'", monitor.monitor("aabbbbbaaaaaaa"), BOTTOM);
    ASSERT_VERDICT("''", monitor.monitor(""), UNKNOWN);
    ASSERT_VERDICT("'bbbbb'", monitor.monitor("bbbbb"), UNKNOWN);
    ASSERT_VERDICT("'bbb'", monitor.monitor("bbb"), UNKNOWN);
    ASSERT_VERDICT("'bbbbba'", monitor.monitor("bbbbba"), TOP);
    ASSERT_VERDICT("'bbbbbaabababa'", monitor.monitor("bbbbbaabababa"), TOP);
    return 0;
}