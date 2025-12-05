//
// Created by 27489 on 2025/10/29.
//

#ifndef COMPILER_DFA_H
#define COMPILER_DFA_H
#include <list>
#include <unordered_map>
#include <set>
#include "../constant/Common.h"
#include <vector>



class DFA{
public:
    std::set<int> states;//×´Ì¬¼¯ºÏ
    std::set<char> chars;//×ÖÄ¸±í
    ReflectOfDFA transitions;//Ó³ÕÕ
    int original_state; //Î¨Ò»³õÌ¬
    std::set<int> final_states;
    DFA();

    DFA(std::set<int> states,std::set<char> chars,
        ReflectOfDFA transitions,
        int original_state);
    DFA(std::set<int> states,std::set<char> chars,
        ReflectOfDFA transitions,
        int original_state, std::set<int> final_states);

    DFA minimizeDFA();
    void printDFA() const;

    bool empty() const;
private:
    void makeDFAComplete();

    std::vector<std::set<int>> initSet();
    bool couldSub(const std::vector<std::set<int>>& set,const std::set<int>& s, int index) const;
    int findBelongSet(const std::vector<std::set<int>>& set, int value) const;
    std::pair<std::set<int>, std::set<int>> getSubSet(const std::set<int>& set,const std::vector<std::set<int> >& totalSet,int index);

    static std::set<int> findNewStatesSet(const std::vector<std::set<int>> &total_set);
    std::set<int> findNewFinalStatesSet(const std::vector<std::set<int>> &total_set) const;
    int findNewOriginalState(const std::vector<std::set<int>> &total_set) const;
    ReflectOfDFA findNewTransitions(const std::vector<std::set<int>> &total_set,const std::set<int>& new_state_set) const;
};


#endif //COMPILER_DFA_H