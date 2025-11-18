//
// Created by 27489 on 2025/10/29.
//

#include "../Utils.h"


#include <algorithm>
#include <iostream>
#include <fstream>
#include <limits>
using namespace std;


NFA Utils::closure(const NFA &a) {
    int new_start=*a.final_states.begin()+1;
    int new_final=new_start+1;

    set<int> new_states=a.states;
    new_states.insert(new_start);
    new_states.insert(new_final);

    ReflectOfNFA newTransitions=a.transitions;
    newTransitions[{*a.final_states.begin(),EPSILON}]={*a.original_state.begin()};

    newTransitions[{new_start,EPSILON}]={*a.original_state.begin(),new_final};
    newTransitions[{*a.final_states.begin(),EPSILON}]={new_final,*a.original_state.begin()};

    return {new_states,a.chars,newTransitions,{new_start},{new_final}};
}

NFA Utils::unionTwoBasicNFA(const NFA &a, const NFA &b) {
    set<char> newChars = a.chars;
    for (char c : b.chars) {
        newChars.insert(c);
    }

    int new_start=max(*a.final_states.begin(),*b.final_states.begin())+1;
    int new_final=new_start+1;
    ReflectOfNFA new_transitions=a.transitions;
    for (const auto& entry:b.transitions) {
        new_transitions[entry.first]=entry.second;
    }
    new_transitions[{new_start,EPSILON}]={*a.original_state.begin(),*b.original_state.begin()};

    new_transitions[{*b.final_states.begin(),EPSILON}]={new_final};
    new_transitions[{*a.final_states.begin(),EPSILON}]={new_final};

    set<int> new_states=a.states;
    new_states.insert(new_start);
    new_states.insert(new_final);
    for (int i: b.states) {
        new_states.insert(i);
    }

    return {new_states,newChars,new_transitions,{new_start},{new_final}};

}

bool Utils::isSubSet(const std::set<int> &subset, const std::set<int> &superset) {
    return std::includes(superset.begin(), superset.end(),
                        subset.begin(), subset.end());
}

std::pair<NFA *, int> Utils::readNFAFile(std::ifstream file) {
    if (!file) {
        std::cout<<"Error opening file"<<std::endl;
        return {};
    }

    int n;
    file >> n;
    if (n <= 0) {
        std::cout << "Invalid n" << std::endl;
        return {};
    }
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    NFA* nfa = new NFA[n];

    for (int i = 0; i < n; i++) {

        file=nfa[i].readNfaUnite(std::move(file));
        if (!file) {
            std::cout<<"Error opening file"<<std::endl;
            return{};
        }
        // nfa[i].output();
    }

    file.close();
    return make_pair(nfa, n);
}

NFA Utils::mergeNFA(NFA *nfa,int cnt) {
    if (cnt <= 0) {

        return {};
    }

    if (cnt == 1) {
        return nfa[0];
    }

    NFA result = nfa[0];
    for (int i = 1; i < cnt; i++) {
        result = mergeTwoNFA(result, nfa[i]);
    }
    return result;
}

NFA Utils::mergeTwoNFA(NFA& a, NFA& b) {
    int maxState = 0;
    if (!a.states.empty()) {
        maxState=*a.states.rbegin();
    }
    int offset=maxState+1;

    std::unordered_map<int,int> bStateMap;
    for (int state:b.states) {
        bStateMap[state]=offset+state;
    }

    std::set<int> newStates=a.states;
    for (const auto& pair:bStateMap) {
        newStates.insert(pair.second);
    }


    int newStartState = newStates.empty() ? 0 : (*newStates.rbegin() + 1);
    newStates.insert(newStartState);


    std::set<char> newChars = a.chars;
    for (char c : b.chars) {
        newChars.insert(c);
    }

    ReflectOfNFA newTransitions = a.transitions;


    for (const auto& entry : b.transitions) {
        int originalFrom = entry.first.first;
        char symbol = entry.first.second;
        const std::list<int>& originalTos = entry.second;

        int newFrom = bStateMap[originalFrom];
        std::list<int> newTos;
        for (int to : originalTos) {
            newTos.push_back(bStateMap[to]);
        }

        newTransitions[{newFrom, symbol}] = newTos;
    }

    for (int start : a.original_state) {
        newTransitions[{newStartState, EPSILON}].push_back(start);
    }


    for (int start : b.original_state) {
        newTransitions[{newStartState, EPSILON}].push_back(bStateMap[start]);
    }


    std::set<int> newFinalStates = a.final_states;
    for (int final : b.final_states) {
        newFinalStates.insert(bStateMap[final]);
    }


    std::set<int> newOriginalStates;
    newOriginalStates.insert(newStartState);


    return {newStates, newChars, newTransitions, newOriginalStates, newFinalStates};
}

// template<typename TransitionType>
// std::set<int> Utils::getEpsilonClosureOfSet(const std::set<int>& status, const TransitionType &transitions) {
//     set<int> result={};
//     for (const auto& entry : status) {
//         result.emplace(entry);
//         list<int> temp=getEpsilonClosure(entry,transitions);
//         if (!temp.empty()) {
//             result.insert(temp.begin(),temp.end());
//         }
//     }
//     return result;
// }

// template<typename TransitionType>
// std::list<int> Utils::getEpsilonClosure(int status, const TransitionType &transitions) {
//     stack<int> s;
//     set<int> r={status};
//     s.push(status);
//     while (!s.empty()) {
//         int state=s.top();
//         s.pop();
//         std::list<int> temp = getNextByChar(state,EPSILON,transitions);
//         if (!temp.empty()) {
//             for (int i : temp) {
//                 s.push(i);
//                 r.emplace(i);
//             }
//         }
//     }
//     list<int> result(r.begin(),r.end());
//     return result;
// }

// std::list<int> Utils::getEpsilonClosure(int status, const ReflectOfDFA &transitions) {
//     stack<int> s;
//     set<int> r={status};
//     s.push(status);
//     while (!s.empty()) {
//         int state=s.top();
//         s.pop();
//         int temp = getNextByChar(state,EPSILON,transitions);
//         if (temp!=NO_MATCH_STATE) {
//             s.push(temp);
//             r.emplace(temp);
//         }
//     }
//     list<int> result(r.begin(),r.end());
//     return result;
// }


// std::set<int> Utils::getNextByCharOfSet(const std::set<int> &status, char c, const ReflectOfDFA &transitions) {
//     set<int> result={};
//     for (const auto& entry : status) {
//         int list = getNextByChar(entry,c,transitions);
//         if (list!=NO_MATCH_STATE) {
//             result.emplace(list);
//         }
//     }
//     return result;
// }

// template<typename TransitionType>
// std::set<int> Utils::getNextByCharOfSet(const std::set<int> &status,char c,const TransitionType& transitions) {
//     set<int> result={};
//     for (const auto& entry : status) {
//         std::list<int> list = getNextByChar(entry,c,transitions);
//         if (!list.empty()) {
//             result.insert(list.begin(),list.end());
//         }
//     }
//     return result;
// }

// int Utils::getNextByChar(int status, char c, const ReflectOfDFA &transitions) {
//     auto it=transitions.find({status,c});
//     if (it!=transitions.end()) {
//         return it->second;
//     }
//     return NO_MATCH_STATE;
// }
//
// template<typename TransitionType>
// std::list<int> Utils::getNextByChar(int status, char c, const TransitionType &transitions) {
//     auto it=transitions.find({status,c});
//     if (it!=transitions.end()) {
//         return it->second;
//     }
//     return {};
// }