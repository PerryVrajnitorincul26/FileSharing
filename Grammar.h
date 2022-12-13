//
// Created by perry on 10/25/22.
//
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <ostream>
#include <istream>
#include <sstream>
#include <random>

using string_list = decltype(std::vector<std::string>());
using key_value = decltype(std::pair<std::string, string_list>());
using my_prod_hash = decltype(std::unordered_map<std::string, string_list>());

inline string_list split(const std::string &s, char delim) {
    string_list result;
    std::stringstream strStrm(s);
    std::string item;

    while (getline(strStrm, item, delim)) {
        result.emplace_back(item);
    }

    return result;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vector){
    for(const auto &i : vector)
    {
        os<<i<<" ";
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const string_list &slist);

std::istream &operator>>(std::istream &is, string_list &slist);

std::ostream &operator<<(std::ostream &os, const my_prod_hash &hsh);

std::istream &operator>>(std::istream &is, std::pair<std::string, string_list> &h_entry);

std::ostream &operator<<(std::ostream &os, const key_value &kval);

std::istream &operator>>(std::istream &is, my_prod_hash &f_hash);

bool operator==(const string_list &lhs, const string_list &rhs);


class Grammar {
public:
    //! @todo make this private
    string_list m_terminale, m_neterminale;
    std::string m_start;
    my_prod_hash m_hash_map;

    friend std::ostream &operator<<(std::ostream &os, const Grammar &grammar);

    friend std::istream &operator>>(std::istream &is, Grammar &grammar);


    Grammar(const std::vector<std::string> &mTerminale, const std::vector<std::string> &mNeterminale,
            const std::string &mStart,
            const my_prod_hash &mHashMap);

    Grammar(const std::vector<std::string> &mTerminale, const std::vector<std::string> &mNeterminale,
            const std::string &mStart);

    Grammar();

    virtual ~Grammar();

    bool validNoOverlap();

    bool validNoForeignTerms();

    bool validNoForeignTerm(const key_value &kval);

    bool stringNoForeignTerm(std::string str);

    bool validStartInProduction();

    bool validProdContainsNonTerm(const key_value &kval);

    bool validHashContainsNonTerm();

    bool validProdContainsStart();

    bool validProductionContent(const std::pair<std::string, string_list> &kval);

    bool validValidHashMap();

    bool isValid();

    bool isRegular();

    bool regularProd(const key_value &kval);

    const std::vector<std::string> &getMTerminale() const;

    void setMTerminale(const std::vector<std::string> &mTerminale);

    const std::vector<std::string> &getMNeterminale() const;

    void setMNeterminale(const std::vector<std::string> &mNeterminale);

    const std::string &getMStart() const;

    void setMStart(const std::string &mStart);

    void setMProdValues(const std::vector<string_list> &mProdValues);

    const my_prod_hash &getMHashMap() const;

    void setMHashMap(const my_prod_hash &mHashMap);

    std::string genWord(std::ostream &os);

    std::string expandProd(const key_value &kval, const std::string &start, std::ostream &os);
};
