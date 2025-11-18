//
// Created by 27489 on 2025/10/29.
//

#include "../DFA.h"
#include <iostream>
#include <utility>

#include "../../utils/Utils.h"

using namespace std;


DFA DFA::minimizeDFA() {
    vector<set<int>> set=initSet();
    bool flag=false;
    while (!flag) {
        flag=true;
        for (int i=0;i<set.size();i++) {
            std::set<int> s = set[i];
            if (couldSub(set,s,i)) {
                std::pair<std::set<int>, std::set<int>> sub_set = getSubSet(s,set,i);
                if (!sub_set.first.empty() && !sub_set.second.empty()) {
                    set.erase(set.begin()+i);
                    set.push_back(sub_set.first);
                    set.push_back(sub_set.second);
                    flag=false;
                }
            }
        }
    }

    std::set<int> new_states_set = this->findNewStatesSet(set);
    int new_original_state = this->findNewOriginalState(set);
    std::set<int> new_final_states_set = this->findNewFinalStatesSet(set);
    ReflectOfDFA new_transitions = this->findNewTransitions(set,new_states_set);

    return {new_states_set,chars,new_transitions,new_original_state,new_final_states_set};
}

std::set<int> DFA::findNewStatesSet(const std::vector<std::set<int>> &total_set) {
    set<int> new_states_set;
    for (int i=0;i<total_set.size();i++) {
        new_states_set.insert(*total_set[i].begin());
    }
    return new_states_set;
}

std::set<int> DFA::findNewFinalStatesSet(const std::vector<std::set<int>> &total_set) const {
    set<int> new_final_states;
    for (int i=0;i<total_set.size();i++) {
        for (int j: final_states) {
            if (total_set[i].count(j)) {
                new_final_states.insert(*(total_set[i].begin()));
                break;
            }
        }
    }
    return new_final_states;
}

int DFA::findNewOriginalState(const std::vector<std::set<int>> &total_set) const {
    for (int i=0;i<total_set.size();i++) {
        if (total_set[i].count(original_state)) {
            return *total_set[i].begin();
        }
    }
    return ERROR;
}

ReflectOfDFA DFA::findNewTransitions(const std::vector<std::set<int>> &total_set,const std::set<int>& new_state_set) const {
    ReflectOfDFA new_transitions=transitions;
    auto it = new_transitions.begin();
    while (it!=new_transitions.end()) {
        if (new_state_set.count(it->first.first)==0) {
            it=new_transitions.erase(it);
        }else {
            int belong_set = findBelongSet(total_set,it->second);
            if (belong_set!=NO_MATCH_SET) {
                it->second=*total_set[belong_set].begin();
            }
            ++it;
        }
    }

    return new_transitions;
}

pair<set<int>, set<int>> DFA::getSubSet(const std::set<int>& set,const std::vector<std::set<int> >& totalSet,int index) {
    int a=NO_STATE;
    int b;
    vector<int> curSet(set.begin(),set.end());
    vector<char> curChars(chars.begin(),chars.end());
    int flag=false;

    for (int i=0;i<curChars.size();i++) {

        unordered_map<int,int> map;
        for (int j=0;j<curSet.size();j++) {
            std::list<int> list=Utils::getNextByChar(curSet[j],curChars[i],transitions);
            int next_state=*(list.begin());
            int belong_set=findBelongSet(totalSet,next_state);
            map[belong_set]=j;
        }
        if (map.size()!=1) {
            for (const auto& pair : map) {
                if (a==NO_STATE) a=pair.second;
                else {
                    b=pair.second;
                    break;
                }
            }
            flag=true;
            break;
        }
    }

    if (!flag) return {};

    std::set<int> set1={curSet[a]};
    curSet.erase(curSet.begin()+a);
    std::set<int> set2(curSet.begin(),curSet.end());

    return std::make_pair(set1,set2);
}

int DFA::findBelongSet(const std::vector<std::set<int> > &set, int value) const {
    for (int i=0;i<set.size();i++) {
        if (set[i].count(value)) {
            return i;
        }
    }
    return NO_MATCH_SET;
}

bool DFA::couldSub(const std::vector<std::set<int> >& set,const std::set<int>& s, int index) const {
    for (char c : chars) {
        std::set<int> t=Utils::getEpsilonClosureOfSetAfterGetNextByCharOfSet(s,c,transitions);
        for (int i=0;i<set.size();i++) {
            // if (i!=index) {
                if (!Utils::isSubSet(t, set[i])) {
                    return true;
                }
            // }
        }
    }
    return false;
}

vector<set<int>> DFA::initSet() {
    vector<set<int>> set;
    set.push_back(final_states);
    std::set<int> temp;
    for (int i:states) {
        if (final_states.count(i)==0) {
            temp.emplace(i);
        }
    }
    set.push_back(temp);
    return set;
}

void DFA::printDFA() const{
    cout << "状态集合: ";
    for (int s : states) cout << s << " ";
    cout << "\n符号集合: ";
    for (char c : chars) cout << c << " ";
    cout << "\n转换规则: " << endl;
    for (const auto& entry : transitions) {
        cout << "(" << entry.first.first << ", '" << entry.first.second << "') → ";
        cout << entry.second << " ";
        cout << endl;
    }
    cout << "初始状态: ";
    cout << original_state << " ";
    cout<< "\n终止状态: ";
    for (int f : final_states) cout << f << " ";
    cout << endl <<endl;
}

DFA::DFA(std::set<int> states, std::set<char> chars, ReflectOfDFA transitions, int original_state, std::set<int> final_states) {
    this->states = std::move(states);
    this->chars = std::move(chars);
    this->transitions = std::move(transitions);
    this->original_state = original_state;
    this->final_states = std::move(final_states);
}

DFA::DFA(std::set<int> states, std::set<char> chars, ReflectOfDFA transitions, int original_state) {
    this->states = std::move(states);
    this->chars = std::move(chars);
    this->transitions = std::move(transitions);
    this->original_state = original_state;
}


