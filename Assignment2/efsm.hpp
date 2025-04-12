/**
 * Defines Extended Finite State Machine.
 */
#pragma once
#include <algorithm>
#include "enable.hpp"
#include <iostream>
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
 * Used to mark state types.
 */
enum state_type {START = (1 << 0), ACCEPTING = (1 << 1), DEFAULT = (1 << 2)};

/**
 * Class to define states.
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
                std::string next_state,
                char input,
                Enable* enable,
                unsigned int increment);
private:
    std::string current_state;
    std::string str_next;
    unsigned int next;
    Transition* next_transition;
    char input;
    Enable* enable;
    unsigned int increment; // How much to add if we take transition (Update)
    friend class EFSM;
};

/**
 * Class used to represent the symbolic states (location, interval)
 */
class SymbolicState {
public:
    SymbolicState(unsigned int state,
                std::string str_state,
                Enable* enable,
                std::unordered_set<SymbolicState*> next,
                SymbolicState* prev);
    ~SymbolicState();
    std::string getStringState() {
        return str_state;
    }
    Enable* getInterval() {
        return interval;
    }
private:
    unsigned int state;
    std::string str_state;
    Enable* interval;
    std::unordered_set<SymbolicState*> next;
    SymbolicState* prev;
    friend class EFSM;
};

class EFSM {
    private:
        unsigned int start_state = __INT_MAX__;
        std::unordered_set<unsigned int> accept_states;
        std::vector<Transition*> adj_list; // Adjacency-list to model all transitions
        std::unordered_map<std::string, unsigned int> convert; // used to convert state representations (string to int)
        unsigned int initial; // Initial register value. Can be overridden when calling EFSM::reach

        /**
         * Function used for checking for valid state.
         */
        inline void handle_state(unsigned int& enumeration, State* st);
    public:
        /**
         * Constructor to define an EFSM.
         *
         * @param states List of state structs
         * @param transitions List of transitions
         * @param initial value for single register
         *
         */
        EFSM(std::vector<State*>& states,
             std::vector<Transition*>& transitions,
             unsigned int initial);

        /**
         * Test if a string is in the language of the EFSM.
         *
         * @param input is the string to test
         *
         * @return true if string is in the language, else return false
         */
        bool verify(std::string input);

        /**
         * Determines the non-empty langauge states starting from
         * @param str_state string representation of start state
         * @param initial initial value of EFSM register
         *
         * Does a Breadth-first search starting from str_state and traverses graph.
         *
         * @return list of symbolic states
         */
        std::vector<SymbolicState*> reach(std::string str_state);
};