//
// Created by perry on 11/18/22.
//

#include "Automaton.h"

#include <utility>

std::string typeToString(State::type val) {
    switch (val) {
        case (State::type::norm):
            return "Normal";
        case (State::type::start):
            return "Start";
        case (State::type::final):
            return "Final";
        default:
            return "Shouldn't be possible";
    }
}

State::type stringToType(std::string val) {
    if (val == "normal")
        return State::type::norm;
    if (val == "start")
        return State::type::norm;
    if (val == "final")
        return State::type::norm;
    return State::type::null;
}

Automaton::Automaton(std::vector<State> copy_from, string_list alphabet) {
    m_state_count = copy_from.size();
    m_state_list = new State[m_state_count];
    std::move(copy_from.begin(), copy_from.end(), m_state_list);
    m_alphabet = std::move(alphabet);
    this->finalize();
}

std::ostream &operator<<(std::ostream &os, const Automaton &automaton) {
    os << "m_state_list: " << automaton.m_state_list << "\n m_state_count: " << automaton.m_state_count
       << " m_alphabet: "
       << automaton.m_alphabet << " m_final: " << automaton.m_final << " m_start: " << automaton.m_start;
    return os;
}

void Automaton::finalize() {
    for (int i = 0; i < m_state_count; ++i) {
        if (m_state_list[i].m_type == State::type::start)
            this->m_start = m_state_list + i;
    }
    for (int i = 0; i < m_state_count; ++i) {
        if (m_state_list[i].m_type == State::type::final)
            this->m_final.push_back(m_state_list + i);
    }
    for (int i = 0; i < m_state_count; ++i) {
        for (int j = 0; j < m_state_count; ++j) {
            
        }
    }

}

State &State::operator=(const State &lhs) {
    m_identifier = lhs.m_identifier;
    m_type = lhs.m_type;
    m_transitions = lhs.m_transitions; //I can hardly believe I didn't have to write an operator for this
    return *this;
}

State::State(State::type mType, std::string mIdentifier, std::vector<transition> &mTransitions) : m_type(
        mType), m_identifier(std::move(mIdentifier)), m_transitions(mTransitions) {}

State::State(State::type mType, std::string mIdentifier) : m_type(mType), m_identifier(std::move(mIdentifier)) {}

std::ostream &operator<<(std::ostream &os, const State &state) {
    os << "m_type: " << typeToString(state.m_type) << "m_identifier: " << state.m_identifier << " m_transitions: "
       << "{" << state.m_transitions << "}";
    return os;
}

std::ostream &operator<<(std::ostream &os, State *state) {
    os << *state;
    return os;
}

std::ostream &operator<<(std::ostream &os, const transition &lhs) {
    os << lhs.first;
    return os;
}
