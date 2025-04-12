/**
 * Monitoring the property
 * \varphi = \neg p \textbf{U} q
 *
 * (Classic property !spawn U init)
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
        'a', // p and q
        'b', // p and !q
        'c', // !p and q
        'd'  // !p and !q
    };

    // Initialize states
    State q0("q0", START);
    states.push_back(&q0);
    State q1("q1", DEFAULT);
    states.push_back(&q1);
    State q2("q2", ACCEPTING);
    states.push_back(&q2);
    // Transitions
    LowerBound always_enabled(0, false);
    Transition t1("q0", "q0", 'b', &always_enabled, 0);
    transitions.push_back(&t1);
    Transition t2("q0", "q1", 'd', &always_enabled, 0);
    transitions.push_back(&t2);
    Transition t3("q0", "q2", 'a', &always_enabled, 0);
    transitions.push_back(&t3);
    Transition t4("q0", "q2", 'c', &always_enabled, 0);
    transitions.push_back(&t4);
    // Self loops
    Transition t5("q1", "q1", 'a', &always_enabled, 0);
    transitions.push_back(&t5);
    Transition t6("q1", "q1", 'b', &always_enabled, 0);
    transitions.push_back(&t6);
    Transition t7("q1", "q1", 'c', &always_enabled, 0);
    transitions.push_back(&t7);
    Transition t8("q1", "q1", 'd', &always_enabled, 0);
    transitions.push_back(&t8);

    Transition t9("q2", "q2", 'a', &always_enabled, 0);
    transitions.push_back(&t9);
    Transition t10("q2", "q2", 'b', &always_enabled, 0);
    transitions.push_back(&t10);
    Transition t11("q2", "q2", 'c', &always_enabled, 0);
    transitions.push_back(&t11);
    Transition t12("q2", "q2", 'd', &always_enabled, 0);
    transitions.push_back(&t12);

    EFSM e(states, transitions, alphabet, 0);

    Monitor monitor(&e);
    ASSERT_VERDICT("'b'", monitor.monitor("b"), UNKNOWN);
    ASSERT_VERDICT("'bbbbb'", monitor.monitor("bbbbb"), UNKNOWN);
    ASSERT_VERDICT("'bbbbbd'", monitor.monitor("bbbbbd"), BOTTOM);
    ASSERT_VERDICT("'bdbbbbd'", monitor.monitor("bdbbbbd"), BOTTOM);
    ASSERT_VERDICT("'dbdbbbbd'", monitor.monitor("dbdbbbbd"), BOTTOM);
    ASSERT_VERDICT("'bacdad'", monitor.monitor("bacdad"), TOP);
    ASSERT_VERDICT("'bbbbcadad'", monitor.monitor("bbbbcadad"), TOP);
    return 0;
}