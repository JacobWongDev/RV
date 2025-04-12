#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>

#ifndef EFSM_H
#define EFSM_H

/**
 * Enabling function to specify that a transition is always enabled.
 *
 * @param vars is an unordered_map containing all EFSM variables.
 *
 * @return true
 */
bool TRUE(std::unordered_map<std::string, unsigned int>& vars);

/**
 * Update function to specify that no updates are performed on variables.
 *
 * @param vars is an unordered_map containing all EFSM variables.
 *
 */
void NOP(std::unordered_map<std::string, unsigned int>& vars);

/**
 * Used to mark state types.
 */
enum state_type {START = (1 << 0), ACCEPTING = (1 << 1), DEFAULT = (1 << 2)};

/**
 * Struct to define states.
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
 * Class to define transitions between states.
 *
 * NOTE: Ensure that current_state and next_states only contain strings that are
 * also passed to EFSM in the states set during instantiation.
 *
 */
class Transition {
public:
    Transition(std::string current_state,
               std::vector<std::string> next_states,
               char input,
               bool (*enable)(std::unordered_map<std::string, unsigned int>&),
               void (*update)(std::unordered_map<std::string, unsigned int>&));
private:
    std::string current_state;
    std::vector<std::string> next_states;
    std::unordered_set<unsigned int> next;
    Transition* next_transition;
    char input;
    bool (*enable)(std::unordered_map<std::string, unsigned int>&);
    void (*update)(std::unordered_map<std::string, unsigned int>&);
    friend class EFSM;
};

/**
 * Internal struct used to represent an epsilon transition
 */
class ETransition {
public:
    ETransition(std::string current_state, std::vector<std::string> next_states);
private:
    std::string current_state;
    std::vector<std::string> next_states;
    friend class EFSM;
};

class EFSM {
private:
    std::unordered_set<unsigned int> start_states;
    std::unordered_set<unsigned int> accept_states;
    std::vector<Transition*> adj_list; // Adjacency-list to model all transitions
    std::vector<std::unordered_set<unsigned int>> ep_list; // list of epsilon transitions
    std::unordered_map<std::string, unsigned int> variables;

    /**
     * Function used for checking for valid state.
     *
     * @param convert map to convert state strings to integers
     */
    inline void handle_state(std::unordered_map<std::string, unsigned int>& convert,
                             unsigned int& enumeration,
                             State* st);
    /**
     * Internal function called recursively to traverse epsilon transitions.
     *
     * @param state current state
     * @param states current set of active states
     */
    void traverse_epsilon(unsigned int state, std::queue<unsigned int>* states);

    /**
     * Internal function used to validate the provided epsilon transitions.
     *
     * If unverified, the user could pass an EFSM with an epsilon transition loop,
     * resulting in an infinite loop.
     */
    void validate_epsilon();

    /**
     * Internal function called recursively to traverse epsilon transitions.
     *
     * @param state current state
     * @param visited set of states already visited
     */
    bool validate_epsilon(unsigned int state, std::unordered_set<unsigned int>& visited);
public:
    /**
     * Constructor to define an EFSM.
     *
     * @param states List of state structs
     * @param transitions List of transitions
     * @param e_transitions List of epsilon transitions
     *
     */
    EFSM(std::vector<State*>& states,
         std::vector<Transition*>& transitions,
         std::vector<ETransition*>& e_transitions,
         std::unordered_map<std::string, unsigned int>& vars);

    /**
     * Initialize first queue. Checks for epsilon transitions from start states.
     *
     * @param active_states set of active states
     */
    void init_verify_queue(std::queue<unsigned int>* active_states);

    /**
     * Test if a string is in the language of the EFSM.
     *
     * @param input is the string to test
     *
     * @return true if string is in the language, else return false
     */
    bool verify(std::string input);
};

#endif