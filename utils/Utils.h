//
// Created by 27489 on 2025/10/29.
//

#ifndef COMPILER_UTILS_H
#define COMPILER_UTILS_H
#include "../class/NFA.h"
#include "../constant/Common.h"
#include <stack>

class Utils {
public:
    static NFA mergeNFA(NFA* nfa,int cnt);

    static NFA mergeTwoNFA(NFA& a, NFA& b);

    static std::pair<NFA*,int> readNFAFile(std::ifstream file);

    //?????????????c???????????
    template<typename TransitionType>
    static std::list<int> getNextByChar(int status,char c,const TransitionType& transitions) {
        auto it=transitions.find({status,c});
        if (it!=transitions.end()) {

            return convertToStateList(it->second);
        }
        return {};
    }

    //????????????c???????????
    template<typename TransitionType>
    static std::set<int> getNextByCharOfSet(const std::set<int> &status,char c,const TransitionType& transitions) {
        std::set<int> result={};
        for (const auto& entry : status) {
            std::list<int> list = getNextByChar(entry,c,transitions);
            if (!list.empty()) {
                result.insert(list.begin(),list.end());
            }
        }
        return result;
    }

        //????????-???
    template<typename TransitionType>
    static std::set<int> getEpsilonClosureOfSet(const std::set<int>& status, const TransitionType &transitions) {
        std::set<int> result={};
        for (const auto& entry : status) {
            result.emplace(entry);
            std::list<int> temp=getEpsilonClosure(entry,transitions);
            if (!temp.empty()) {
                result.insert(temp.begin(),temp.end());
            }
        }
        return result;
    }

    //????????????c????-???
    template<typename TransitionType>
    static std::set<int> getEpsilonClosureOfSetAfterGetNextByCharOfSet(const std::set<int> &status,char c,const TransitionType& transitions) {
        return getEpsilonClosureOfSet(getNextByCharOfSet(status,c,transitions),transitions);
    }


    static NFA closure(const NFA& a);
    static NFA unionTwoBasicNFA(const NFA& a, const NFA& b);
    static bool isSubSet(const std::set<int>& subset, const std::set<int>& superset);
private:

    template<typename TransitionType>
    static std::list<int> getEpsilonClosure(int status,const TransitionType& transitions) {
        std::stack<int> s;
        std::set<int> r={status};
        s.push(status);
        while (!s.empty()) {
            int state=s.top();
            s.pop();
            std::list<int> temp = getNextByChar(state,EPSILON,transitions);
            if (!temp.empty()) {
                for (int i : temp) {
                    s.push(i);
                    r.emplace(i);
                }
            }
        }
        std::list<int> result(r.begin(),r.end());
        return result;
    }

    // ????·Ú1?????? int
    static std::list<int> convertToStateList(int value) {
        return {value};
    }

    // ????·Ú2?????? std::list<int>
    static std::list<int> convertToStateList(const std::list<int>& value) {
        return value;
    }

};


#endif //COMPILER_UTILS_H