#include <iostream>
#include <fstream>
#include "Grammar.h"
#include "Automaton.h"

int main() {
    string_list c = {"hey", "ce", "debug", "misto"};
    string_list d = {"da", "bro", "e", "fain"};
    Grammar *a = new Grammar({"ta", "tb", "tc"}, {"na", "nb"}, "na", {{"na", {"tbna", "ta"}}});


    std::cout << *a << "\n" << a->isValid() << a->isRegular() << a->genWord(std::cout);
    //transition b({"wow", {nullptr}});
    //std::vector<transition> c = {b};
    //auto test = new State(State::type::norm,"normal",c);
    //auto test2 = new State(State::type::start,"wow",c);
    //auto test3 = new State(State::type::final,"fin",c);
    //auto lmao = new Automaton({*test,*test2,*test3},{"wow","frate"});
    //std::cout<<*lmao;
//   auto a = new Automaton({{}})

}
