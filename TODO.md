# TODO

Items that should be worked on based on feedback from the prof.

## Assignment 2

- Handle error cases: multiple start states, use of convert map with characters not in map already
(caused by user using state strings not defined in state vector). Ex:

```
unsigned int idx = convert.at(transition.current_state); // TODO: ERROR if unrecognized state
e->next = convert.at(transition.next_state);// TODO: ERROR if unrecognized state
```
