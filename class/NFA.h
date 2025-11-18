//
// Created by 27489 on 2025/10/29.
//

#ifndef COMPILER_NFA_H
#define COMPILER_NFA_H
#include <list>
#include <set>
#include <string>
#include <unordered_map>

#include "DFA.h"


class NFA {
public:
    std::set<int> states;//状态集合
    std::set<char> chars;//字母表
    ReflectOfNFA transitions;//映照
    std::set<int> original_state;//初态
    std::set<int> final_states;
    /*
     *构造函数
     */
    NFA();
    // 构造函数：委托给静态函数的返回值
    explicit NFA(const std::string& filePath) : NFA(buildFromFile(filePath)){};
    NFA(std::set<int> states,std::set<char> chars,ReflectOfNFA transitions,
       std::set<int> original_state);
    NFA(std::set<int> states,std::set<char> chars,ReflectOfNFA transitions,
       std::set<int> original_state,std::set<int> final_states);
    ~NFA() = default;
    /*
     * NFA确定化
     */
    DFA determineNFA();
    std::ifstream readNfaUnite(std::ifstream file);
    /*
     *打印NFA
     *
     */
    void printNFA() const;
    /*
     *重载+运算符，实现NFA的连接操作
     */
    NFA operator+(const NFA& a) const;

private:
    static NFA buildFromFile(const std::string& filePath);

};


#endif //COMPILER_NFA_H