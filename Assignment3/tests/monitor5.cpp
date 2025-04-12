/**
 * Using custom example.
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
        'c',
        'd'
    };

    // Initialize states
    State q0("q0", START);
    states.push_back(&q0);
    State q1("q1", DEFAULT);
    states.push_back(&q1);
    State q2("q2", DEFAULT);
    states.push_back(&q2);
    State q3("q3", ACCEPTING);
    states.push_back(&q3);
    // Transitions
    LowerBound always_enabled(0, false);
    Transition t1("q0", "q0", 'a', &always_enabled, 1);
    transitions.push_back(&t1);
    Interval zero_four(0, false, 4, false);
    Transition t2("q0", "q1", 'c', &zero_four, 0);
    transitions.push_back(&t2);
    Interval three_five(3, false, 5, false);
    Transition t3("q0", "q2", 'b', &three_five, 0);
    transitions.push_back(&t3);
    Interval zero_two(0, false, 2, false);
    Transition t4("q0", "q3", 'd', &zero_two, 0);
    transitions.push_back(&t4);
    Transition t5("q1", "q1", 'd', &always_enabled, 1);
    transitions.push_back(&t5);
    Interval four_ten(4, false, 10, false);
    Transition t6("q1", "q3", 'b', &four_ten, 0);
    transitions.push_back(&t6);
    Transition t7("q2", "q2", 'a', &always_enabled, 1);
    transitions.push_back(&t7);
    Interval five_ten(5, false, 10, false);
    Transition t8("q2", "q3", 'c', &five_ten, 0);
    transitions.push_back(&t8);
    // Self-loops on q3
    Transition t9("q3", "q3", 'a', &always_enabled, 0);
    transitions.push_back(&t9);
    Transition t10("q3", "q3", 'b', &always_enabled, 0);
    transitions.push_back(&t10);
    Transition t11("q3", "q3", 'c', &always_enabled, 0);
    transitions.push_back(&t11);
    Transition t12("q3", "q3", 'd', &always_enabled, 0);
    transitions.push_back(&t12);
    EFSM e(states, transitions, alphabet, 0);

    Monitor monitor(&e);
    ASSERT_VERDICT("''", monitor.monitor(""), UNKNOWN);
    ASSERT_VERDICT("'aaaa'", monitor.monitor("aaaa"), UNKNOWN);

    ASSERT_VERDICT("'c'", monitor.monitor("c"), UNKNOWN);
    ASSERT_VERDICT("'ac'", monitor.monitor("ac"), UNKNOWN);
    ASSERT_VERDICT("'aaaac'", monitor.monitor("aaaac"), UNKNOWN);
    ASSERT_VERDICT("'aaaaac'", monitor.monitor("aaaaac"), BOTTOM);
    ASSERT_VERDICT("'aaaacb'", monitor.monitor("aaaacb"), TOP);
    ASSERT_VERDICT("'aacddb'", monitor.monitor("aacddb"), TOP);
    ASSERT_VERDICT("'aacddddddddb'", monitor.monitor("aacddddddddb"), TOP);
    ASSERT_VERDICT("'aacdddddddddb'", monitor.monitor("aacdddddddddb"), BOTTOM);

    ASSERT_VERDICT("'aaab'", monitor.monitor("aaab"), UNKNOWN);
    ASSERT_VERDICT("'aaaab'", monitor.monitor("aaaab"), UNKNOWN);
    ASSERT_VERDICT("'aaaaab'", monitor.monitor("aaaaab"), UNKNOWN);
    ASSERT_VERDICT("'aaaaaab'", monitor.monitor("aaaaaab"), BOTTOM);
    ASSERT_VERDICT("'aaaaabc'", monitor.monitor("aaaaabc"), TOP);
    ASSERT_VERDICT("'aaabaac'", monitor.monitor("aaabaac"), TOP);
    ASSERT_VERDICT("'aaabaaac'", monitor.monitor("aaabaaac"), TOP);
    ASSERT_VERDICT("'aaaaabaaaaac'", monitor.monitor("aaaaabaaaaac"), TOP);
    ASSERT_VERDICT("'aaaaabaaaaaac'", monitor.monitor("aaaaabaaaaaac"), BOTTOM);
    return 0;
}