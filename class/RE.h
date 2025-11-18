//
// Created by 27489 on 2025/11/11.
//

#ifndef COMPILER_RE_H
#define COMPILER_RE_H
#include <string>
#include <stack>
#include "NFA.h"


class RE {
    std::string re;
    int state;

public:
    NFA transformReToNFA();
    RE();
    explicit RE(const std::string &re);
    ~RE()=default;
private:
    NFA getBasicNFA(char c);

    void calculate(std::stack<NFA>& nfaStack, std::stack<char>& operatorStack);
    static bool isChar(char c);
    static bool isOperator(char c);
    static int precedence(char c);
    static std::string preProcess(const std::string& re);
};


#endif //COMPILER_RE_H