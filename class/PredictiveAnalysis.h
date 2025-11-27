//
// Created by 27489 on 2025/11/26.
//

#ifndef COMPILER_PREDICTIVEANALYSIS_H
#define COMPILER_PREDICTIVEANALYSIS_H
#include <stack>

#include "Grammar.h"


class PredictiveAnalysis {
public:
    std::stack<char> analysis_stack;
    Grammar& grammar;

    explicit PredictiveAnalysis(Grammar &grammar)
        : grammar(grammar) {
    }

    void analysis(const std::string& file_path);
};


#endif //COMPILER_PREDICTIVEANALYSIS_H