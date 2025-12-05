//
// Created by 27489 on 2025/12/2.
//

#include "../LRAnalysis.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <utility>
using namespace std;

int getIndex(Grammar& grammar,char c) {
    if (grammar.isTerminal(c)) {
        for (int i=0;i<grammar.terminalSymbols.size();i++) {
            if (c==grammar.terminalSymbols[i]) return i;
        }
    }else if (grammar.isNonTerminal(c)) {
        for (int i=0;i<grammar.nonTerminalSymbols.size();i++) {
            if (c==grammar.nonTerminalSymbols[i]) return i;
        }
    }else {
        return  -1;
    }
}

int advance(int& index, const string& input,Grammar& grammar,char &c) {

    if (index+1<input.size()) {
        c=input[++index];
        return getIndex(grammar,c)+1;
    }
    return -1;
}
// std::stack<int> statue_stack;
// std::stack<char> char_stack;


// 检查双层 unordered_map 中键是否存在
bool isKeyExist(
    const unordered_map<int, std::unordered_map<int, Action>>& map,
    int outerKey,
    int innerKey
) {
    auto outerIt = map.find(outerKey);
    if (outerIt == map.end()) {
        return false;
    }
    const auto& innerMap = outerIt->second;
    return innerMap.find(innerKey) != innerMap.end();
}

void LRAnalysis::analysis(const std::string &input) {
    int index=0;
    char c=input[0];
    int a =getIndex(grammar,input[0])+1;
    statue_stack.push(0);
    //char_stack.push('#');
    int s=statue_stack.top();
    char c_top='0';

    while (action_table[s][a].first!='a') {
        cout<<s<<" "<<c_top<<" "<<c;
        if (isKeyExist(action_table,s,a)) {
            pair<char, int> pair = action_table[s][a];
            if (pair.first=='s') {
                statue_stack.push(pair.second);
                char_stack.push(c);
                 s=statue_stack.top();
                 c_top=char_stack.top();


                cout<<"进栈 "<<s<<" "<<c_top<<endl;
                a = advance(index,input,grammar,c);
            }
            else {
                std::pair<char, std::string> production = grammar.production_map[pair.second];
                cout<<"出栈"<<production.second.size()<<"个符号和状态  ";
                for (int i=0;i<production.second.size();i++) {
                    statue_stack.pop();
                    char_stack.pop();
                }
                s=statue_stack.top();
                // TODO
                int mapped = go_to_table[s][getIndex(grammar,production.first)];
                statue_stack.push(mapped);
                char_stack.push(production.first);
                s=statue_stack.top();
                c_top=char_stack.top();
                cout<<"进栈 "<<s<<" "<<c_top<<production.first<<" -> "<<production.second <<endl;
            }
        }else {
            cout<<"ERROR\n";
            return;
        }
    }
    cout<<s<<" "<<c_top<<" acc 接收成功\n";
}


std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

LRAnalysis::LRAnalysis(const std::string& filePath,Grammar grammar) {
    this->grammar=std::move(grammar);
// 清空表，避免残留数据
    action_table.clear();
    go_to_table.clear();

    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filePath);
    }

    std::string line;
    // -------------------------- 解析 ActionTable --------------------------
    // 第一步：读取ActionTable项数（第一行仅数字）
    if (!std::getline(file, line)) {
        throw std::runtime_error("文件为空，无Action表项数");
    }
    std::string actionCountStr = trim(line);
    if (actionCountStr.empty()) {
        throw std::runtime_error("Action表项数行为空");
    }
    int actionItemCount = std::stoi(actionCountStr);

    // 第二步：读取actionItemCount行Action表项
    for (int i = 0; i < actionItemCount; ++i) {
        if (!std::getline(file, line)) {
            throw std::runtime_error("Action表项数不足，期望" + std::to_string(actionItemCount) + "行，实际读取" + std::to_string(i) + "行");
        }
        std::string validLine = trim(line);
        if (validLine.empty()) {
            throw std::runtime_error("第" + std::to_string(i+1) + "行Action表项为空");
        }

        // 解析格式：状态 终结符(INT) 操作（如 0 2 s4 / 1 0 acc / 2 0 r5）
        std::istringstream iss(validLine);
        int state;
        int term;  // 终结符改为int类型（核心修改点）
        std::string opStr;

        if (!(iss >> state >> term >> opStr)) {
            throw std::runtime_error("Action表行格式错误: " + line);
        }

        // 解析操作类型
        Action action;
        if (opStr == "acc") {
            // 接受操作：('a', -1)
            action = std::make_pair('a', -1);
        } else if (opStr.front() == 's') {
            // 移进操作：s后接数字（如s4 → ('s',4)）
            int target = std::stoi(opStr.substr(1));
            action = std::make_pair('s', target);
        } else if (opStr.front() == 'r') {
            // 归约操作：r后接数字（如r5 → ('r',5)）
            int rule = std::stoi(opStr.substr(1));
            action = std::make_pair('r', rule);
        } else {
            throw std::runtime_error("未知操作类型: " + opStr + "（行：" + line + "）");
        }

        // 存入Action表（内层key为int类型的term）
        action_table[state][term] = action;
        // grammar.action_table[state][grammar.terminalSymbols[term]] = action;
    }

    // -------------------------- 解析 GoToTable --------------------------
    // 第一步：读取GoToTable项数
    if (!std::getline(file, line)) {
        throw std::runtime_error("无GoTo表项数行");
    }
    std::string gotoCountStr = trim(line);
    if (gotoCountStr.empty()) {
        throw std::runtime_error("GoTo表项数行为空");
    }
    int gotoItemCount = std::stoi(gotoCountStr);

    // 第二步：读取gotoItemCount行GoTo表项
    for (int i = 0; i < gotoItemCount; ++i) {
        if (!std::getline(file, line)) {
            throw std::runtime_error("GoTo表项数不足，期望" + std::to_string(gotoItemCount) + "行，实际读取" + std::to_string(i) + "行");
        }
        std::string validLine = trim(line);
        if (validLine.empty()) {
            throw std::runtime_error("第" + std::to_string(i+1) + "行GoTo表项为空");
        }

        // 解析格式：状态 非终结符(INT) 目标状态（如 0 1 1）
        std::istringstream iss(validLine);
        int state;
        int nonTerm;  // 非终结符改为int类型（核心修改点）
        int targetState;

        if (!(iss >> state >> nonTerm >> targetState)) {
            throw std::runtime_error("GoTo表行格式错误: " + line);
        }

        // 存入GoTo表（内层key为int类型的nonTerm）
        go_to_table[state][nonTerm] = targetState;
        // grammar.go_to_table[state][grammar.nonTerminalSymbols[nonTerm]] = targetState;
    }

    file.close();
    // std::cout << "LR表解析成功！" << std::endl;
}

void LRAnalysis::printActionTable() {

    // 打印ActionTable
    std::cout << "\n===== Action Table =====" << std::endl;
    for (const auto& statePair : action_table) {
        int state = statePair.first;
        for (const auto& termPair : statePair.second) {
            int term = termPair.first;  // int类型的终结符
            const Action& action = termPair.second;
            std::cout << "状态: " << state << ", 终结符: " << term
                      << ", 操作: " << action.first << ", 数值: " << action.second << std::endl;
        }
    }

}

void LRAnalysis::printGoToTable() {
    // 打印GoToTable
    std::cout << "\n===== GoTo Table =====" << std::endl;
    for (const auto& statePair : go_to_table) {
        int state = statePair.first;
        for (const auto& nonTermPair : statePair.second) {
            int nonTerm = nonTermPair.first;  // int类型的非终结符
            int target = nonTermPair.second;
            std::cout << "状态: " << state << ", 非终结符: " << nonTerm
                      << ", 目标状态: " << target << std::endl;
        }
    }
}
