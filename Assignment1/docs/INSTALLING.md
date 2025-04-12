
# Installation

To build all executables, simply use the Makefile. Running

```make```

Will build all executables. Please note that compilations use C++23 DEFAULT.

# What is included in the build?

Each of the following executables simulates a finite state machine and 
executes some tests to verify behaviour:

- simple_dfa simulates a simple DFA (From Figure 1.22 Pg. 44 in Sipser)
- simple_nfa simulates a simple NFA (From Example 1.30 Pg. 51 in Sipser)
- simple_nfa_ep simulates a simple NFA including epsilon transitions (From Example 1.33 Pg. 52 in Sipser)
- simple_efsm simulates a simple EFSM (defined in docs/simple_efsm.jpg)
- complex_nfa simulates a complicated NFA with multiple epsilon transitions (defined in docs/complex_nfa.jpg)
- complex_efsm simulates a complicated EFSM with multiple variables and update functions (defined in docs/complex_efsm.jpg)

It also includes a test program error_test, which will verify exceptions are thrown in certain cases.
