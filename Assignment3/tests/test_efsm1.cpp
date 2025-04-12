/**
 * Using DFA from:
 *  Figure 1.22 Pg. 44 in "Introduction to the Theory of Computation" 2nd. Edition by Sipser.
 */
#include "../monitor.hpp"
#include "../test.hpp"
 /**
  * Main Function.
  */
int main() {
  std::vector<State*> states;
  std::vector<Transition*> transitions;
  std::vector<char> alphabet = {'0', '1'};

  // Initialize states
  State q("q", START);
  states.push_back(&q);
  State q0("q0", DEFAULT);
  states.push_back(&q0);
  State q00("q00", DEFAULT);
  states.push_back(&q00);
  State q001("q001", ACCEPTING);
  states.push_back(&q001);
  // Transitions
  LowerBound always_enabled(0, false);
  Transition t1("q", "q", '1', &always_enabled, 0);
  transitions.push_back(&t1);
  Transition t2("q", "q0", '0', &always_enabled, 0);
  transitions.push_back(&t2);
  Transition t3("q0", "q", '1', &always_enabled, 0);
  transitions.push_back(&t3);
  Transition t4("q0", "q00", '0', &always_enabled, 0);
  transitions.push_back(&t4);
  Transition t5("q00", "q00", '0', &always_enabled, 0);
  transitions.push_back(&t5);
  Transition t6("q00", "q001", '1', &always_enabled, 0);
  transitions.push_back(&t6);
  Transition t7("q001", "q001", '1', &always_enabled, 0);
  transitions.push_back(&t7);
  Transition t8("q001", "q001", '0', &always_enabled, 0);
  transitions.push_back(&t8);

  EFSM e(states, transitions, alphabet, 0);
  ASSERT("'0110'", e.verify("0010"), true);
  ASSERT("'1001'", e.verify("1001"), true);
  ASSERT("'001'", e.verify("001"), true);
  ASSERT("'11110011111'", e.verify("11110011111"), true);
  ASSERT("'11'", e.verify("11"), false);
  ASSERT("'0000'", e.verify("0000"), false);

  EFSM* complement = e.complement();
  ASSERT("'0110'", complement->verify("0010"), false);
  ASSERT("'1001'", complement->verify("1001"), false);
  ASSERT("'001'", complement->verify("001"), false);
  ASSERT("'11110011111'", complement->verify("11110011111"), false);
  ASSERT("'11'", complement->verify("11"), true);
  ASSERT("'0000'", complement->verify("0000"), true);
  return 0;
}