//
// Created by perry on 10/25/22.
//

#include "Grammar.h"
#include <iostream>
#include <sstream>
#include <algorithm>

Grammar::Grammar(const string_list &mTerminale, const string_list &mNeterminale,
                 const std::string &mStart,
                 const std::unordered_map<std::string, string_list> &mHashMap) : m_terminale(mTerminale),
                                                                                 m_neterminale(mNeterminale),
                                                                                 m_start(mStart),
                                                                                 m_hash_map(mHashMap) {
}

Grammar::Grammar(const string_list &mTerminale, const string_list &mNeterminale,
                 const std::string &mStart) : m_terminale(mTerminale),
                                              m_neterminale(mNeterminale),
                                              m_start(mStart) {

}

Grammar::Grammar() {
}

Grammar::~Grammar() {

}

const std::vector<std::string> &Grammar::getMTerminale() const {
    return m_terminale;
}

void Grammar::setMTerminale(const std::vector<std::string> &mTerminale) {
    m_terminale = mTerminale;
}

const std::vector<std::string> &Grammar::getMNeterminale() const {
    return m_neterminale;
}

void Grammar::setMNeterminale(const std::vector<std::string> &mNeterminale) {
    m_neterminale = mNeterminale;
}

const std::string &Grammar::getMStart() const {
    return m_start;
}

void Grammar::setMStart(const std::string &mStart) {
    m_start = mStart;
}

void Grammar::setMProdValues(const std::vector<string_list> &mProdValues) {
}

const my_prod_hash &Grammar::getMHashMap() const {
    return m_hash_map;
}

void Grammar::setMHashMap(const std::unordered_map<std::string, string_list> &mHashMap) {
    //m_hash_map = mHashMap;
}

std::ostream &operator<<(std::ostream &os, const Grammar &grammar) {
    os << " m_terminale: " << grammar.m_terminale << " m_neterminale: " << grammar.m_neterminale << " m_start: "
       << grammar.m_start << "\n m_hash_map: " << grammar.m_hash_map;
    return os;
}


std::istream &operator>>(std::istream &is, Grammar &grammar) {
    is >> grammar.m_neterminale;
    is >> grammar.m_terminale;
    is >> grammar.m_start;
    is >> grammar.m_hash_map;
    return is;
}

bool Grammar::isValid() {
    return (!m_terminale.empty()) && (!m_neterminale.empty()) && validNoOverlap() && validNoForeignTerms() &&
           validStartInProduction() &&
           validProdContainsStart() && validValidHashMap();
}

//Likely going to be unused, this checks if ALL entries in production lead to nonterminals
bool Grammar::validProductionContent(const std::pair<std::string, string_list> &kval) {
    if (std::find(m_neterminale.begin(), m_neterminale.end(), kval.first) != m_neterminale.end()) {
        return kval.second == m_terminale;
    } else {
        return false;
    }
}

bool Grammar::validValidHashMap() {
    //If nonterm and term overlap return false;
    if (!validNoOverlap()) return false;
    if (!validStartInProduction()) return false;
    if (!validHashContainsNonTerm()) return false;
    return true;
}

bool Grammar::validNoOverlap() {
    for (const auto &i: m_neterminale) {
        if (std::find(m_terminale.begin(), m_terminale.end(), i) != m_terminale.end())
            return false;
    }
    return true;
}

bool Grammar::validStartInProduction() {
    for (const auto &i: m_hash_map) {
        if (m_start == i.first)
            return true;
    }
    return false;
}

///
bool Grammar::validProdContainsNonTerm(const key_value &kval) {
    for (const auto &vals: kval.second) {
        bool flag = false;
        for (const auto &i: m_terminale) {
            if (vals.find(i) != vals.npos) {
                flag = true;
                break;
            }
        }
        if (flag == false)
            return false;
    }
    return true;
}

bool Grammar::validProdContainsStart() {
    for (const auto &kval: m_hash_map) {
        if (kval.first.find(m_start) != kval.first.npos)
            return true;
    }
    return false;
}

bool Grammar::validHashContainsNonTerm() {
    for (const auto &kval: m_hash_map) {
        if (!validProdContainsNonTerm(kval))
            return false;
    }
    return true;
}

bool Grammar::isRegular() {
    for (const auto &it: m_hash_map) {
        if (!regularProd(it))
            return false;
    }
    return true;
}

/*!
 * First looks for terminal at the begining of each production, assuming one is found evaluates the remainder.
 * @param kval
 * @return
 */
bool Grammar::regularProd(const key_value &kval) {
    for (const auto &i: kval.second) {
        bool flag_has_nonterm = false;
        std::string substr = "";
        for (const auto &j: m_terminale) {
            if (i.starts_with(j)) {
                flag_has_nonterm = true;
                substr = i.substr(j.length());
                break;
            }

        }
        if (flag_has_nonterm) {
            if (substr == "" || std::find(m_neterminale.begin(), m_neterminale.end(), substr) != m_neterminale.end())
                break;
        } else {
            return false;
        }
        return false;

    }
    return true;
}

bool Grammar::validNoForeignTerms() {
    for (const auto &i: m_hash_map)
        if (!validNoForeignTerm(i)) return false;
    return true;
}

bool Grammar::validNoForeignTerm(const key_value &kval) {
    if (!stringNoForeignTerm(kval.first)) return false;
    for (const auto &i: kval.second)
        if (!stringNoForeignTerm(i)) return false;
    return true;
}

bool Grammar::stringNoForeignTerm(std::string str) {
    string_list cpy = m_terminale;
    cpy.insert(cpy.end(), m_neterminale.begin(), m_neterminale.end());
    bool flag_rem = false;
    do {
        for (const auto &i: cpy) {
            if (str.length() >= i.length() && str.starts_with(i)) {
                str = str.substr(i.length());
                flag_rem = true;
                break;
            }

        }
    } while (flag_rem && str != "");

    return str == "";
}

std::string Grammar::genWord(std::ostream &os) {
    std::string retVal = m_start;
    string_list keys;

    for (const auto &i: m_hash_map)
        keys.push_back(i.first);
    std::random_device rd;
    std::mt19937 gen(rd());
    bool flag_replaced = false;
    do {
        flag_replaced = false;
        string_list found_keys;
        for (const auto &i: keys) {
            if (retVal.find(i) != retVal.npos)
                found_keys.push_back(i);
        }
        if (!found_keys.empty()) {
            std::uniform_int_distribution<> distrib(0, found_keys.size()-1);
            /**
             * The single part of the program that might "theoretically" benefit from having used a hash map for storing
             * productions I lay defeated, something like std::vector<std::pair<std::string,std::string>> would've undoubtedly
             * been both easier to implement and more efficient (given small-ish grammars)
            **/
            auto rVal = distrib(gen);
            retVal = expandProd({keys[rVal], m_hash_map[keys[rVal]]}, retVal, os);
            flag_replaced = true;
        }

    } while (flag_replaced);
    return retVal;

}

std::string Grammar::expandProd(const key_value &kval, const std::string &start, std::ostream &os) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(0, kval.second.size()-1);
    auto rVal = distrib(gen);
    auto pos = start.find(kval.first);
    if (pos == start.npos) {
        throw std::invalid_argument("Given production not found in string");
    }
    os<<"On \""<<start<<"\" Applied :["<<kval.first<<"] -> "<<kval.second[rVal]<<"\n";
    return start.substr(0, pos) + kval.second[rVal] + start.substr(pos+kval.first.length(),start.length());
}


std::ostream &operator<<(std::ostream &os, const string_list &slist) {
    for (const auto &i: slist) {
        os << i << " ";
    }
    os << "\n";
    return os;
}

std::istream &operator>>(std::istream &is, string_list &slist) {
    std::string temp;
    std::getline(is, temp);
    slist = split(temp, ' ');
    return is;
}

std::ostream &operator<<(std::ostream &os, const my_prod_hash &hsh) {
    for (const auto &i: hsh) {
        os << i << "\n";
    }
    os << "\n";
    return os;
}

std::istream &operator>>(std::istream &is, std::pair<std::string, string_list> &h_entry) {
    is >> h_entry.first;
    is >> h_entry.second;//Calls previously defined operator. This will likely break if only a single word was given
    return is;
}

std::istream &operator>>(std::istream &is, my_prod_hash &f_hash) {
    std::pair<std::string, string_list> lmao;
    ///CLION doesn't know how to handle EOF therefore we must use another character to end input
    while (!is.eof() && !(is.peek() == '`')) {
        is >> lmao;
        f_hash.emplace(lmao);
    }
    return is;
}

bool operator==(const string_list &lhs, const string_list &rhs) {
    for (const auto &i: lhs) {
        if (std::find(rhs.begin(), rhs.end(), i) == rhs.end())
            return false;
    }
    return true;
}

std::ostream &operator<<(std::ostream &os, const key_value &kval) {
    for (const auto &i: kval.second) {
        os << "[" << kval.first << "] -> " << i << std::endl;
    }
    return os;
}
