
# Overview

## Building project

To build all executables, simply use the Makefile. Running

```make```

Will build all executables. Please note that compilations use C++23 by **DEFAULT**.

## What is included in the build?

The library simulates a **DETERMINISTIC** Extended Finite State Machine.
We run the following tests on the library:

To verify the library's ability to find non-empty language states:
- simple1 simulates a custom EFSM (See: docs/simple1.jpg)
- simple2 simulates a custom EFSM (See: docs/simple2.jpg)
- complex1 simulates a custom EFSM (See: docs/complex1.jpg)

To verify the library's ability to determine if some sequence of inputs is
in the EFSM's language or not:
- test_efsm1 simulates DFA from textbook (See: Sipser Figure 1.22 Pg. 44)
- test_efsm2 simulates a custom EFSM (See: docs/test_efsm2.jpg)

It also includes a test program error_test, which will verify exceptions are thrown in certain cases.

## Future work

I think that there is definitely room for improvement the current algorithm for
finding non-empty language states.

Initially, I tried using DFS but quickly realized that it can recurse indefinitely immediately
in some cases without exploring much of the state space.

So instead I then switched to BFS and this yielded better results. It still has drawbacks though,
since for any of these traversal methods, if there are cycles we will continue traversing that cycle
repeatedly unless we get to a point where one or more transitions open up for acceptance.

One particular algorithm I was thinking about using in the future is the following:

1. Start by determining the set of cycles in the EFSM. This can be done using Johnson's algorithm:

https://www.cs.tufts.edu/comp/150GA/homeworks/hw1/Johnson%2075.PDF

2. Next, make a list of all paths which do not have cycles, ending in an accept state.

3. For Steps 1, 2, we are ignoring the enabling functions on edges. Iterate over all paths from Step 2 to find
one that may work.

If you come to an enabling function that does not work, backtrack and see if there are any cycles that you can loop over
a certain number of times to get past that edge.

The particular method I was thinking of using to solve this is Bezout's identity.

If we have n cycles before the troublesome edge, see if some linear combination of these cycles can result in a
register value allowing us to pass it.

**Example:**

For instance, this could clearly be done for the EFSM in docs/simple1. After trying to go directly to the accepting state 3,
backtrack to use the cycle on 2 (+3). Solving

3x = 11
3x = 10

yields non-integer solutions. As such, we need to backup further to look for more cycles to leverage. If there arent any, then
there is no solution. Now add in cycle on 1 (+2)

2x + 3y = 11
2x + 3y = 10

Using Bezout's, we get (x = 2, y = 3) or (x = 5, y = 0).

The beauty of this approach is that, if it worked in theory, then we have to do significantly less trial-and-error
compared to the BFS approach. Furthermore, it can give us a definitive answer when we cannot find a solution in some cases.
