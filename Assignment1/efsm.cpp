#include "efsm.hpp"

bool TRUE(std::unordered_map<std::string, unsigned int>& vars)  { return true; }

void NOP(std::unordered_map<std::string, unsigned int>& vars) {}

Transition::Transition(std::string current_state,
                       std::vector<std::string> next_states,
                       char input,
                       bool (*enable)(std::unordered_map<std::string, unsigned int>&),
                       void (*update)(std::unordered_map<std::string, unsigned int>&)) {
    this->current_state = current_state;
    this->next_states = next_states;
    this->input = input;
    this->enable = enable;
    this->update = update;
    this->next_transition = nullptr;
}

ETransition::ETransition(std::string current_state, std::vector<std::string> next_states) {
    this->current_state = current_state;
    this->next_states = next_states;
}

inline void EFSM::handle_state(std::unordered_map<std::string, unsigned int>& convert,
                               unsigned int& enumeration,
                               State* st) {
    if((st->type & 0xFFF8) != 0 || st->type == 0x0) { // Invalid state
        throw std::runtime_error("ERROR: State is invalid; expected DEFAULT, START or ACCEPTING!");
    } else {
        if(st->type & DEFAULT) {
            if(st->type & (START | ACCEPTING)) {
                throw std::runtime_error("ERROR: State is invalid; cannot be DEFAULT and START or ACCEPTING!");
            }
        } else {
            if(st->type & ACCEPTING)
                accept_states.insert(enumeration);
            if(st->type & START)
                start_states.insert(enumeration);
        }
    }
    convert.insert(std::pair<std::string, unsigned int>(st->name, enumeration++));
}

void EFSM::validate_epsilon() {
    std::unordered_set<unsigned int> visited;
    for(unsigned int c = 0; c < ep_list.size(); c++) {
        if(!validate_epsilon(c, visited)) {
            throw std::runtime_error("ERROR: Invalid epsilon transitions. Loop in graph found!");
        }
    }
}

bool EFSM::validate_epsilon(unsigned int state, std::unordered_set<unsigned int>& visited) {
    if(!visited.insert(state).second) return false;
    for(auto& next: ep_list[state]) {
        if(!validate_epsilon(next, visited)) return false;
    }
    visited.erase(state);
    return true;
}

EFSM::EFSM(std::vector<State*>& states,
           std::vector<Transition*>& transitions,
           std::vector<ETransition*>& e_transitions,
           std::unordered_map<std::string, unsigned int>& vars) {
    std::unordered_map<std::string, unsigned int> convert;
    unsigned int enumeration = 0;
    /**
     * To simplify internal processing, all state strings are enumerated starting from 0.
     *
     * The string representation is not used internally; from here onwards, states are now integers.
     */
    for(auto& state: states) {
        /**
         * User specifies state type through bit patterns
         * its either DEFAULT or (START exclusive or ACCEPTING)
         * So we are looking for
         *
         * (START) 100, (ACCEPTING) 010, (START & ACCEPTING) 110, (DEFAULT) 001
         */
        handle_state(convert, enumeration, state);
    }
    if(accept_states.empty() || start_states.empty()) {
        throw std::runtime_error("ERROR: No start state or accept states!");
    }
    // Resize lists to accomodate number of states
    adj_list.resize(enumeration, nullptr);
    ep_list.resize(enumeration);
    // Process transitions
    for(auto& transition: transitions) {
        for(auto& state: transition->next_states) {
            transition->next.insert(convert.at(state));// TODO: ERROR if unrecognized state
        }
        unsigned int idx = convert.at(transition->current_state); // TODO: ERROR if unrecognized state
        Transition* t = adj_list[idx];
        if(t == nullptr) {
            adj_list[idx] = transition;
        } else {
            while(t->next_transition != nullptr) {
                t = t->next_transition;
            }
            t->next_transition = transition;
        }
    }
    // Process epsilon transitions
    for(auto& etransition: e_transitions) {
        unsigned int current_state = convert.at(etransition->current_state);
        for(auto& state: etransition->next_states) {
            ep_list[current_state].insert(convert.at(state)); // TODO: convert error handling here.
        }
    }
    // Check epsilon transitions for loops
    validate_epsilon();
    // Make a copy of variables
    variables = vars;
}

void EFSM::init_verify_queue(std::queue<unsigned int>* states) {
    for(unsigned int state: start_states)
        traverse_epsilon(state, states);
}

void EFSM::traverse_epsilon(unsigned int state, std::queue<unsigned int>* states) {
    states->push(state);
    for(auto& s: ep_list[state])
        traverse_epsilon(s, states);
}

bool EFSM::verify(std::string input) {
    std::unordered_map<std::string, unsigned int> v(variables);
    std::queue<unsigned int> s1, s2;
    std::queue<unsigned int>* active = &s1, *next = &s2, *temp;
    init_verify_queue(active);
    for(char c: input) {
        while(!active->empty()) {
            Transition* t = adj_list[active->front()];
            active->pop();
            while(t != nullptr && t->input != c) {
                t = t->next_transition;
            }
            /*
                Either there is no transition from this character, or we found it.

                If no transition found, we dont add any states to the queue
                active states and the run dies. Otherwise, if enabled, take
                transition and add to a2.
            */
            if(t != nullptr && t->enable(v)) {
                for(unsigned int k: t->next) {
                    traverse_epsilon(k, next);
                }
                t->update(v);
            }
        }
        temp = active;
        active = next;
        next = temp;
    }
    while(!active->empty()) {
        if(accept_states.contains(active->front()))
            return true;
        else
            active->pop();
    }
    return false;
}