
# Overview

## Building project

To build all executables, run the Makefiles like so:

```make -f Makefile```
```make -f Makefile.test```

This will build all executables. Please note that compilations use C++23 by **DEFAULT**.

## What is included in the build?

This library has two different versions: PROD and TESTING.

Depending on which Makefile is used for compilation, different include guards will be triggered
inside the project and different interfaces will be exposed.

- using ```Makefile``` will produce the production build in ```prod/```
- using ```Makefile.test``` will produce the testing build in ```test/```

The reason for this is that the code used for testing grants the user access certain objects that
are internal to the library, which shouldn't be available in the production build.

### The "prod" build

Intended to test the main purpose of the project: Monitoring a property described as an EFSM against a
provided trace to produce an LTL3 verdict.

The included tests are
- monitor1 which verifies correct behaviour on the property φ = □p
- monitor2 which verifies correct behaviour on the property φ = ♢p
- monitor3 which verifies correct behaviour on the property φ = ¬pUq
- monitor4 which verifies correct behaviour on the property (EFSM) in docs/monitor4.jpeg
- monitor5 which verifies correct behaviour on the property (EFSM) in docs/monitor5.jpeg

### The "test" build

Intended to test elements of the project that support monitoring.

To verify that the library can correctly complement an EFSM, and determine if a trace is in
the language of the property:
- test_efsm1 simulates DFA from textbook (See: Sipser Figure 1.22 Pg. 44)
- test_efsm2 simulates a custom EFSM (See: docs/test_efsm2.jpg)

To verify that the library can correctly find the reach set of an EFSM:
- simple1 simulates a custom EFSM (See: docs/simple1.jpeg)
- simple2 simulates a custom EFSM (See: docs/simple2.jpeg)
- complex1 simulates a custom EFSM (See: docs/complex1.jpeg)
- complex2 simulates a custom EFSM (See: docs/complex2.jpeg)

To verify the correct behaviour when intersecting enabling functions:
- test_enable executes intersections between Intervals, LowerBounds and UpperBounds.

Finally, to check if exceptions are correctly thrown:
- error_test

## Future work
- Add more test cases to error_test.cpp
- Switch from forward reachability to backwards reachability in EFSM::reach
