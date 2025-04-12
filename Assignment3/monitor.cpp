#include "monitor.hpp"

_Transition::_Transition(unsigned int next,
                         char input,
                         Enable* enable,
                         unsigned int increment) {
    this->next = next;
    this->next_transition = nullptr;
    this->input = input;
    this->enable = enable;
    this->increment = increment;
}

Transition::Transition(std::string current,
                       std::string next,
                       char input,
                       Enable* enable,
                       unsigned int increment) {
    if(enable == nullptr) throw new std::runtime_error("Error: enable pointing to nullptr");
    this->current = current;
    this->next = next;
    this->input = input;
    this->enable = enable;
    this->increment = increment;
}

_SymbolicState::_SymbolicState(unsigned int state,
                               Enable* enable,
                               std::unordered_set<_SymbolicState*> next,
                               _SymbolicState* prev) {
    this->state = state;
    this->interval = enable;
    this->next = next;
    this->prev = prev;
}

_SymbolicState::~_SymbolicState() {
    for(_SymbolicState* state: next) {
        delete state;
    }
    delete interval;
}

void EFSM::handle_state(std::unordered_map<std::string,
                        unsigned int>& convert_str_i,
                        unsigned int& enumeration,
                        State* st) {
    if(st->name == "sink")
        throw std::runtime_error("ERROR: State cannot have name 'sink', this is a reserved keyword!");
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
    #ifdef TESTING
    convert_i_str.insert(std::pair<unsigned int, std::string>(enumeration, st->name));
    #endif
    convert_str_i.insert(std::pair<std::string, unsigned int>(st->name, enumeration));
    enumeration++;
}

EFSM::EFSM(std::vector<State*>& states,
           std::vector<Transition*>& transitions,
           std::vector<char>& alphabet,
           unsigned int initial) {
    /**
     * To simplify internal processing, all state strings are enumerated starting from 0.
     *
     * The string representation is not used internally; from here onwards, states are now integers.
     */
    unsigned int enumeration = 0;
    std::unordered_map<std::string, unsigned int> convert_str_i;
    this->alphabet = std::unordered_set<char>(alphabet.begin(), alphabet.end());
    for(auto& state: states) {
        /**
         * User specifies state type through bit patterns
         * So we are looking for
         *
         * (START) 100, (ACCEPTING) 010, (START & ACCEPTING) 110, (DEFAULT) 001
         */
        handle_state(convert_str_i, enumeration, state);
    }
    if(states.empty())
        throw std::runtime_error("ERROR: No states!");
    if(accept_states.empty())
        throw std::runtime_error("ERROR: No accept states!");
    if(start_state == __INT_MAX__)
        throw std::runtime_error("ERROR: No start state!");
    if(alphabet.empty())
        throw std::runtime_error("ERROR: Alphabet cannot be empty!");

    // Resize lists to accomodate number of states
    // ++enumeration to add 1 for sink state
    adj_list.resize(++enumeration, nullptr);

    #ifdef TESTING
    convert_i_str.insert(std::pair<unsigned int, std::string>(enumeration-1, "sink"));
    #endif
    convert_str_i.insert(std::pair<std::string, unsigned int>("sink", enumeration-1));

    // Process user-defined transitions
    for(auto& tr: transitions) {
        unsigned int idx = convert_str_i[tr->current];
        _Transition* t = adj_list[idx];
        if(t == nullptr) {
            adj_list[idx] = new _Transition(convert_str_i[tr->next], tr->input, tr->enable, tr->increment);
        } else {
            while(t->next_transition != nullptr) {
                t = t->next_transition;
            }
            t->next_transition = new _Transition(convert_str_i[tr->next], tr->input, tr->enable, tr->increment);
        }
    }

    /**
     * Need to complete the EFSM in order to support easy complementing.
     *
     * Add transitions to sink-state from all states
     * This includes self-loops on the sink-state
     */
    for(int i = 0; i < enumeration; i++) {
        _Transition* t = adj_list[i];
        if(t != nullptr) { // Some transitions exist from state i on some input
            std::unordered_set<char> sink_chars = this->alphabet;
            sink_chars.erase(t->input);
            while(t->next_transition != nullptr) {
                t = t->next_transition;
                sink_chars.erase(t->input);
            }
            for(char c: sink_chars) {
                t->next_transition = new _Transition(enumeration - 1, c, &always, 0);
                t = t->next_transition;
            }
        } else { // No transitions exist from state i for any input
            t = new _Transition(enumeration - 1, alphabet[0], &always, 0);
            adj_list[i] = t;
            for(int i = 1; i < alphabet.size(); i++) {
                t->next_transition = new _Transition(enumeration - 1, alphabet[i], &always, 0);
                t = t->next_transition;
            }
        }
    }
    // Initialize single register
    this->initial = initial;
}

EFSM::~EFSM() {
    if(root) {
        if(comp != nullptr) delete comp;
        // Delete all transitions
        for(int i = 0; i < adj_list.size(); i++) {
            _Transition* t = adj_list[i];
            _Transition* temp;
            while(t != nullptr) {
                temp = t->next_transition;
                delete t;
                t = temp;
            }
        }
    }
}

bool EFSM::verify(std::string input) {
    unsigned int r = initial;
    unsigned int current_state = start_state;
    for(char c: input) {
        if(!alphabet.contains(c))
            throw std::runtime_error("ERROR: Unexpected character in trace!");
        _Transition* t = adj_list[current_state];
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

std::unordered_set<unsigned int> EFSM::reach() {
    std::unordered_set<unsigned int> non_empty_lang_states;
    std::queue<_SymbolicState*> q;
    unsigned int iters = 0;
    _SymbolicState* current = new _SymbolicState(start_state, new LowerBound(0, false), {}, nullptr);
    _SymbolicState* head = current;
    q.push(current);
    while(iters++ < MAX_ITERS_SEARCH && !q.empty()) {
        current = q.front();
        q.pop();
        if(accept_states.contains(current->state)) {
            _SymbolicState* temp = current;
            while(temp != nullptr) {
                non_empty_lang_states.insert(temp->state);
                temp = temp->prev;
            }
        }
        _Transition* t = adj_list[current->state];
        while(t != nullptr) {
            Enable* e = current->interval->intersect(*t->enable);
            if(e != nullptr) {
                e->shift(t->increment);
                _SymbolicState* next = new _SymbolicState(t->next, e, {}, current);
                current->next.insert(next);
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
    return non_empty_lang_states;
}


#ifdef PROD
EFSM::EFSM(unsigned int start_state,
           std::unordered_set<unsigned int> accept_states,
           std::vector<_Transition*> adj_list,
           std::unordered_set<char> alphabet,
           unsigned int initial) {
    this->start_state = start_state;
    this->accept_states = accept_states;
    this->adj_list = adj_list;
    this->alphabet = alphabet;
    this->initial = initial;
    this->root = false;
}

EFSM* EFSM::complement() {
    std::unordered_set<unsigned int> accept_states;
    for(int i = 0; i < adj_list.size(); i++) {
        if(!this->accept_states.contains(i))
            accept_states.insert(i);
    }
    comp = new EFSM(
        this->start_state,
        accept_states,
        this->adj_list,
        this->alphabet,
        this->initial
    );
    return comp;
}

Monitor::Monitor(EFSM* property) {
    this->property = new EFSM(
        property->start_state,
        property->reach(),
        property->adj_list,
        property->alphabet,
        property->initial
    );
    EFSM* c = property->complement();
    this->c_property = new EFSM(
        c->start_state,
        c->reach(),
        c->adj_list,
        c->alphabet,
        c->initial
    );
}
#endif

#ifdef TESTING
EFSM::EFSM(std::unordered_map<unsigned int, std::string> convert_i_str,
           unsigned int start_state,
           std::unordered_set<unsigned int> accept_states,
           std::vector<_Transition*> adj_list,
           std::unordered_set<char> alphabet,
           unsigned int initial) {
    this->convert_i_str = convert_i_str;
    this->start_state = start_state;
    this->accept_states = accept_states;
    this->adj_list = adj_list;
    this->alphabet = alphabet;
    this->initial = initial;
    this->root = false;
}

EFSM* EFSM::complement() {
    std::unordered_set<unsigned int> accept_states;
    for(int i = 0; i < adj_list.size(); i++) {
        if(!this->accept_states.contains(i))
            accept_states.insert(i);
    }
    comp = new EFSM(
        this->convert_i_str,
        this->start_state,
        accept_states,
        this->adj_list,
        this->alphabet,
        this->initial
    );
    return comp;
}

Monitor::Monitor(EFSM* property) {
    this->property = new EFSM(
        property->convert_i_str,
        property->start_state,
        property->reach(),
        property->adj_list,
        property->alphabet,
        property->initial
    );
    EFSM* c = property->complement();
    this->c_property = new EFSM(
        c->convert_i_str,
        c->start_state,
        c->reach(),
        c->adj_list,
        c->alphabet,
        c->initial
    );
}

void EFSM::print_reach() {
    std::unordered_set<unsigned int> non_empty_lang_states = reach();
    std::cout << "Non-empty language states are: " << std::endl;
    for(unsigned int s : non_empty_lang_states) {
        std::cout << convert_i_str[s] << ", ";
    }
    std::cout << std::endl;
}
#endif

Monitor::~Monitor() {
    delete property;
    delete c_property;
}

VERDICT Monitor::monitor(std::string trace) {
    bool basic = property->verify(trace);
    bool comp = c_property->verify(trace);
    if(basic && comp) {
        return UNKNOWN;
    } else if(!basic) {
        return BOTTOM;
    } else if(!comp) {
        return TOP;
    } else {
        throw std::runtime_error("Monitoring resulted in impossible verdict!");
    }
}