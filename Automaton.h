//
// Created by perry on 11/18/22.
//
#pragma once

#include "Grammar.h"
#include <string>
#include <vector>
#include <ostream>

///Putin urat :(
class State;

using transition = decltype(std::pair<std::string, std::vector<State *>>());

std::ostream &operator<<(std::ostream &os, const transition &lhs);

///This approach is only faster assuming we re-use the automata. Likely a bad approach for regex and the like.
class State {
public:
    enum class type {
        null, norm, start, final
    } m_type;

    std::string m_identifier;
    std::vector<transition> m_transitions;

    State(type mType, std::string mIdentifier, std::vector<transition> &mTransitions);

    State(type mType, std::string mIdentifier);

    State() = default; //Should do fine as "empty" constructor -m_type

    State &operator=(const State &);

    friend std::ostream &operator<<(std::ostream &os, const State &state);

    //Workaround for templated output function declared in Grammar.h
    friend std::ostream &operator<<(std::ostream &os, State *state);

};


class Automaton {
public:
    explicit Automaton(std::vector<State> copy_from, string_list alphabet);

    friend std::ostream &operator<<(std::ostream &os, const Automaton &automaton);

    void finalize();

private:
    //Ensures contiguity, another option would be to use a vector of unique_ptr which points to a contiguous location in memory
    //On second thought a properly resized vector should've done the trick too
    State *m_state_list;
    int m_state_count;
    string_list m_alphabet;
    std::vector<State *> m_final;
    State *m_start;
public:
};


