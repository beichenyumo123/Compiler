//
// Created by 27489 on 2025/11/9.
//

#ifndef COMPILER_TEST_H
#define COMPILER_TEST_H
#include "../class/DFA.h"
#include "../class/NFA.h"


class Test {

public:
    static std::ifstream getFilePath();
    static DFA testDetermineNFA();
    static DFA testMinimizeDFA();
    static NFA testMergeNFA();
    static void testContact();
    static void testUnion();
    static void testClosure();
};


#endif //COMPILER_TEST_H