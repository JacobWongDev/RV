#ifndef EFSM_H_
#define EFSM_H_

#include "enable.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
 * Defines the maximum number of iterations that can be performed in EFSM::reach
 *
 * If there are cycles, the breadth-first search can continue to execute without bound,
 * so we want to stop this from happening. Technically speaking, this could prohibit our
 * code from finding viable solutions, but if there is no solution then it will iterate indefinitely.
 */
#define MAX_ITERS_SEARCH 100

/**
 * Defines different verdicts that can be returned by Monitor::monitor
 *
 * These are the standard LTL3 verdicts (?,⊥,T)
 */
enum VERDICT {UNKNOWN, BOTTOM, TOP};

/**
 * Used to mark state types.
 */
enum state_type {
    START = (1 << 0),
    ACCEPTING = (1 << 1),
    DEFAULT = (1 << 2)
};

/**
 * Public class to define states.
 */
class State {
public:
    State(std::string name, unsigned short type) : name(name), type(type) {};
private:
    std::string name;
    unsigned short type;
    friend class EFSM;
};

/**
 * Internal library class to represent a transition available in this EFSM.
 */
class _Transition {
public:
    _Transition(unsigned int next,
                char input,
                Enable* enable,
                unsigned int increment);
private:
    unsigned int next;
    _Transition* next_transition;
    char input;
    Enable* enable;
    unsigned int increment;
    friend class EFSM;
    friend class MonitorEFSM;
};

/**
 * Public class to define a transition in this EFSM.
 */
class Transition {
public:
    Transition(std::string current,
               std::string next,
               char input,
               Enable* enable,
               unsigned int increment);
private:
    std::string current;
    std::string next;
    char input;
    Enable* enable;
    unsigned int increment;
    friend class EFSM;
};

/**
 * Class used to represent the symbolic states (location, interval)
 *
 * Used when searching the EFSM for non-empty language states.
 *
 * non-empty language states = states in our EFSM such that there
 * exists a possible path to an accepting state.
 */
class _SymbolicState {
public:
    _SymbolicState(unsigned int state,
                   Enable* enable,
                   std::unordered_set<_SymbolicState*> next,
                   _SymbolicState* prev);
    ~_SymbolicState();
private:
    unsigned int state;
    Enable* interval;
    std::unordered_set<_SymbolicState*> next;
    _SymbolicState* prev;
    friend class EFSM;
};

/**
 * Defines Extended Finite State Machine.
 *
 * EFSMs are described in the work
 *
 * "Automatic Functional Test Generation Using The Extended
 *  Finite State Machine Model" by Kwang-Ting Cheng and A.S. Krishnakumar
 *
 * For this assignment, and for simplicity, the EFSMs in this project only support
 * the use of *one register* which is a Natural number.
 *
 * Additionally, the only supported operation on the register when taking a transition
 * is incrementing the register value by another Natural number.
 */
class EFSM {
private:
    /**
     * Defines the start state for the EFSM
     */
    unsigned int start_state = __INT_MAX__;
    /**
     * Defines the accept states for the EFSM
     */
    std::unordered_set<unsigned int> accept_states;
    /**
     * A vector of linked lists. Contains all transitions in our EFSM
     * (Essentially just an adjacency list for the graph)
     */
    std::vector<_Transition*> adj_list;
    /**
     * Alphabet for the EFSM.
     */
    std::unordered_set<char> alphabet;
    /**
     * Initial register value.
     */
    unsigned int initial;
    /**
     * Stores a pointer to the complemented version of this EFSM.
     */
    EFSM* comp = nullptr;
    /**
     * Static instance of LowerBound used for completion transitions.
     * Use for convenience for transitions with no restrictions on register.
     */
    inline static LowerBound always{0, false};
    /**
     * Used to identify the root EFSM instance.
     *
     * The EFSM itself represents a property, and users can simply call EFSM::verify
     * to check if a given string is in the language of the property.
     *
     * However, the main purpose of this class is to support monitoring properties described
     * as an EFSM. As such, this requires creating 3 new EFSMs from the one passed during monitoring
     *
     * One is the complement of the current EFSM, and the next two are used by the monitor to generate
     * a verdict. Each of these EFSMs use the exact same adjacency list for transitions (on the heap), the
     * main difference is that the accept states are different.
     *
     * As such, only the root EFSM will do memory cleanup for the adjacency list. This flag is used to mark
     * who is responsible for cleanup.
     */
    bool root = true;
    /**
     * Function used for checking for valid state.
     *
     * Determines if the given state is valid by checking
     * @param st ->type and ->name.
     */
    void handle_state(std::unordered_map<std::string, unsigned int>& convert_str_i,
        unsigned int& enumeration,
        State* st);
    /**
    * Internal function used to determine the non-empty langauge states
    * in this EFSM.
    *
    * Does a Breadth-first search starting from initial state (forwards reachability).
    *
    * Since this library supports EFSMs with a single, monotonically increasing register
    * (natural number), this significantly complicates finding the reach set.
    *
    * Each transition can have some restriction on that register, so in other words, transitions
    * can be enabled or disabled based on the value of r, creating complex scenarios where
    * transitions are enabled if a cycle is taken a certain number of times.
    *
    * As such, the reach set is computed with an iteration limit- that is, we search for a certain
    * number of iterations and then quit.
    *
    * See Assignment2/docs/INSTALLING for some potential ideas on improving this...
    *
    * TODO: Switch to backwards reachability.
    *
    * @return list of states (integer form).
    */
    std::unordered_set<unsigned int> reach();
    #ifdef TESTING
    /**
     * Used for debugging+testing.
     *
     * Useful for printing out states in their string format, rather than integer format.
     */
    std::unordered_map<unsigned int, std::string> convert_i_str;
    /**
     * Constructor for two purposes
     * 1. Creating a complemented EFSM
     * 2. Creating a EFSM for monitoring
     */
    EFSM(std::unordered_map<unsigned int, std::string> convert_i_str,
         unsigned int start_state,
         std::unordered_set<unsigned int> accept_states,
         std::vector<_Transition*> adj_list,
         std::unordered_set<char> alphabet,
         unsigned int initial);
    #endif
    #ifdef PROD
    /**
     * Constructor for two purposes
     * 1. Creating a complemented EFSM
     * 2. Creating a EFSM for monitoring
     */
    EFSM(unsigned int start_state,
         std::unordered_set<unsigned int> accept_states,
         std::vector<_Transition*> adj_list,
         std::unordered_set<char> alphabet,
         unsigned int initial);
    /**
     * Returns the complemented EFSM.
     *
     * @return EFSM instance
     */
    EFSM* complement();
    #endif
    friend class Monitor;
public:
    /**
     * Constructor to define an EFSM.
     *
     * @param states List of State objects
     * @param transitions List of Transition objects
     * @param alphabet List of characters expected in trace
     * @param initial value for single register
     *
     */
    EFSM(std::vector<State*>& states,
         std::vector<Transition*>& transitions,
         std::vector<char>& alphabet,
         unsigned int initial);
    /**
     * Destructor.
     *
     * Only root EFSM instance will clear memory.
     */
    ~EFSM();
    /**
     * Test if a string is in the language of the EFSM.
     *
     * @param input is the string to test
     *
     * @return true if string is in the language, else return false.
     */
    bool verify(std::string input);

    #ifdef TESTING
    /**
     * For testing correct operation of EFSM::reach.
     */
    void print_reach();
    /**
     * Returns the complemented EFSM.
     *
     * @return EFSM instance
     */
    EFSM* complement();
    #endif
};

/**
 * Class to represent a Monitor over an EFSM.
 *
 * Monitoring method is described in the work:
 *
 * "Runtime Verification for LTL and TLTL"
 *  by Andreas Bauer, Martin Leucker and Christian Schallhart.
 *
 * Specifically, the procedure to generate an LTL3 monitor is followed
 * in section 2.3
 *
 * Given: property we want to monitor (given as EFSM)
 * 1. Complement property (essentially just complement accept states).
 * 2. Find non-empty language states of EFSM (property) and complemented EFSM
 *    (complemented property)
 * 3. Create a new EFSM for both property and complemented property with the
 *    same transitions, except the accept states for these two are the non-empty
 *    language states found in step 2.
 * 4. Verdicts are described by relation at the bottom of page 14:11
 */
class Monitor {
public:
    /**
     * Constructor to create a new monitor.
     *
     * @param property used to define the target property to monitor
     */
    Monitor(EFSM* property);
    /**
     * Destructor.
     */
    ~Monitor();
    /**
     * Generate LTL3 verdict evaluating property on trace.
     *
     * @returns LTL3 verdict for property (given during instantiation)
     * evaluated on @param trace
     */
    VERDICT monitor(std::string trace);
private:
    EFSM* property;
    EFSM* c_property;
};

#endif
