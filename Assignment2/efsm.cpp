#include "efsm.hpp"

Transition::Transition(std::string current_state,
                        std::string next_state,
                        char input,
                        Enable* enable,
                        unsigned int increment) {
    this->current_state = current_state;
    this->str_next = next_state;
    this->input = input;
    this->enable = enable;
    this->next_transition = nullptr;
    this->increment = increment;
}

SymbolicState::SymbolicState(unsigned int state,
    std::string str_state,
    Enable* enable,
    std::unordered_set<SymbolicState*> next,
    SymbolicState* prev) {
    this->state = state;
    this->str_state = str_state;
    this->interval = enable;
    this->next = next;
    this->prev = prev;
}

SymbolicState::~SymbolicState() {
    for(SymbolicState* state: next) {
        delete state;
    }
    delete interval;
}

inline void EFSM::handle_state(unsigned int& enumeration,
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
                start_state = enumeration;
        }
    }
    convert.insert(std::pair<std::string, unsigned int>(st->name, enumeration++));
}

EFSM::EFSM(std::vector<State*>& states,
            std::vector<Transition*>& transitions,
            unsigned int initial) {
    unsigned int enumeration = 0;
    /**
     * To simplify internal processing, all state strings are enumerated starting from 0.
     *
     * The string representation is not used internally; from here onwards, states are now integers.
     */
    for(auto& state: states) {
        /**
         * User specifies state type through bit patterns
         * So we are looking for
         *
         * (START) 100, (ACCEPTING) 010, (START & ACCEPTING) 110, (DEFAULT) 001
         */
        handle_state(enumeration, state);
    }
    if(states.empty())
        throw std::runtime_error("ERROR: No states!");
    if(accept_states.empty())
        throw std::runtime_error("ERROR: No accept states!");
    if(start_state == __INT_MAX__)
        throw std::runtime_error("ERROR: No start state!");

    // Resize lists to accomodate number of states
    adj_list.resize(enumeration, nullptr);

    // Process transitions
    for(auto& transition: transitions) {
        transition->next = convert.at(transition->str_next);
        if(transition->enable == nullptr)
            throw new std::runtime_error("Error: enable pointing to nullptr");
        unsigned int idx = convert.at(transition->current_state); // ERROR if unrecognized state
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
    // Initialize single register
    this->initial = initial;
}

bool EFSM::verify(std::string input) {
    unsigned int r = initial;
    unsigned int current_state = start_state;
    for(char c: input) {
        Transition* t = adj_list[current_state];
        while(t != nullptr and t->input != c) {
            t = t->next_transition;
        }
        if(t != nullptr && t->enable->check_enable(r)) {
            // Found a transition that is enabled
            current_state = t->next;
            r += t->increment;
        } else {
            // either transition not enabled, or we couldn't
            // find the transition for this character.
            return false;
        }
    }
    if(accept_states.contains(current_state))
        return true;
    else
        return false;
}

std::vector<SymbolicState*> EFSM::reach(std::string str_state) {
    std::queue<SymbolicState*> q;
    std::vector<SymbolicState*> accepted_run;
    unsigned int iters = 0;
    Enable* e1 = new LowerBound(0, false);
    unsigned int state = convert.at(str_state);
    SymbolicState* current = new SymbolicState(state, str_state, e1, {}, nullptr);
    SymbolicState* head = current;
    q.push(current);
    while(!q.empty()) {
        if(++iters >= MAX_ITERS_SEARCH) break;
        current = q.front();
        q.pop();
        if(accept_states.contains(current->state)) { // end traversal here!
            while(current != nullptr) {
                accepted_run.push_back(current);
                current = current->prev;
            }
            std::reverse(accepted_run.begin(), accepted_run.end());
            return accepted_run;
        }
        Transition* t = adj_list[current->state];
        while(t != nullptr) {
            // std::cout << "Intersecting " << current->interval->toString() << " and " << t->enable->toString() << std::endl;
            Enable* e = current->interval->intersect(*t->enable);
            if(e != nullptr) {
                e->shift(t->increment);
                SymbolicState* next = new SymbolicState(t->next, t->str_next, e, {}, current);
                current->next.insert(next);
                // std::cout << "debug: Adding path from " << current->str_state << " to " << t->str_next << std::endl;
                // std::cout << "Increment is " << t->increment << std::endl;
                // std::cout << e->toString() << std::endl;
                q.push(next);
            }
            t = t->next_transition;
        }
    }

    /**
     * Either we ran out of 'time' to find a solution, or there isn't one.
     *
     * Cleanup memory!
     */
    delete head;
    return {};
}
