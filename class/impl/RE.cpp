//
// Created by 27489 on 2025/11/11.
//

#include "../RE.h"

#include <iostream>
#include <ostream>
#include <stack>

#include "../../constant/Common.h"
#include "../../utils/Utils.h"

NFA RE::transformReToNFA() {
    std::stack<NFA> nfaStack;
    std::stack<char> operatorStack;
    for (char c : re)
    {
        if (isChar(c))
        {
            NFA nfa=getBasicNFA(c);
            nfaStack.push(nfa);
        }
        else if (isOperator(c))
        {
            if (!operatorStack.empty())
            {
                if (operatorStack.top()=='(')
                {
                    operatorStack.push(c);
                }
                else
                {
                    while ( !operatorStack.empty() && operatorStack.top()!='('&& precedence(operatorStack.top())>=precedence(c))
                    {
                        calculate(nfaStack,operatorStack);
                    }
                    operatorStack.push(c);
                }

            }
            else
            {
                operatorStack.push(c);
            }
        }
        else
        {
            if (c=='(') operatorStack.push(c);
            if (c==')')
            {
                while (!operatorStack.empty() && operatorStack.top()!='(')
                {
                    calculate(nfaStack,operatorStack);
                }
                operatorStack.pop();
            }
        }
    }
    calculate(nfaStack,operatorStack);
    return nfaStack.top();
}

void RE::calculate(std::stack<NFA>& nfaStack, std::stack<char>& operatorStack) {
    char c=operatorStack.top();
    operatorStack.pop();
    switch (c) {
        case '|': {

            NFA a = nfaStack.top();

            nfaStack.pop();
            NFA b = nfaStack.top();

            nfaStack.pop();
            NFA nfa=Utils::unionTwoBasicNFA(b,a);

            nfaStack.push(nfa);
            state+=2;
            break;
        }
        case '*': {

            NFA a=nfaStack.top();

            nfaStack.pop();
            NFA b=Utils::closure(a);


            nfaStack.push(b);
            state+=2;
            break;
        }
        case '&': {

            NFA a = nfaStack.top();

            nfaStack.pop();
            NFA b = nfaStack.top();

            nfaStack.pop();
            NFA c=b+a;

            nfaStack.push(c);
            break;
        }
        default: {
            break;
        }
    }

}

NFA RE::getBasicNFA(char c) {
    std::set<int> states={state,state+1};//状态集合
    std::set<char> chars={c};//字母表
    ReflectOfNFA transitions;//映照
    transitions[{state,c}]={state+1};

    std::set<int> original_state={state};//初态
    std::set<int> final_states={state+1};

    state+=2;
    return {states,chars,transitions,original_state,final_states};
}

bool RE::isChar(char c) {
    switch (c) {
        case '|':
        case '*':
        case '&':
        case '(':
        case ')':
            return false;
        default:
            return true;
    }
}

bool RE::isOperator(char c) {
    switch (c) {
        case '|':
        case '*':
        case '&':
            return true;
    }
    return false;
}

int RE::precedence(char c) {
    switch (c) {
        case '*':
            return 3;
        case '&':
            return 2;
        case '|':
            return 1;
    }
}

RE::RE() {
    this->re="";
    state=0;
}

//预处理
std::string RE::preProcess(const std::string& re) {
    std::string res;
    int length = re.length();
    for (int i = 0; i < length; i++) {
        // 先将当前字符加入结果（优化写法：避免在每个分支重复 res+=re[i]）
        res += re[i];

        if (i + 1 < length) {
            char next = re[i+1];
            char current = re[i];

            // 判断是否需要插入 '&'
            // 情况1: 下一个是字母
            // 情况2: 下一个是左括号 '('
            if (isalpha(next) || next == '(') {
                // 仅当当前字符是 字母、')' 或 '*' 时，才插入 '&'
                if (isalpha(current) || current == ')' || current == '*') {
                    res += "&";
                }
            }
        }
    }
    return res;
}

RE::RE(const std::string &re) {
    this->re=preProcess(re);
    //std::cout <<this->re<<std::endl;
    state=0;
}
