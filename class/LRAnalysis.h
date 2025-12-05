//
// Created by 27489 on 2025/12/2.
//

#ifndef COMPILER_LRANALYSIS_H
#define COMPILER_LRANALYSIS_H
#include <stack>

#include "Grammar.h"

class LRAnalysis {
public:
    Grammar grammar;

    std::unordered_map<int, std::unordered_map<int,Action>> action_table;
    std::unordered_map<int, std::unordered_map<int,int>> go_to_table;

    std::stack<int> statue_stack;
    std::stack<char> char_stack;


    LRAnalysis(const std::string& filePath,Grammar grammar);
    void printActionTable();
    void printGoToTable();
    void analysis(const std::string& input);

};


#endif //COMPILER_LRANALYSIS_H