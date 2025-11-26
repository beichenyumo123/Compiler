//
// Created by 27489 on 2025/11/25.
//

#ifndef COMPILER_GRAMMARUTILS_H
#define COMPILER_GRAMMARUTILS_H
#include <vector>

#include "../class/Grammar.h"
#include "../constant/GrammarCommon.h"


class GrammarUtils {
public:
    static FirstSet getFirstSet(const Grammar& grammar);
    static FollowSet getFollowSet(const Grammar& grammar, FirstSet& first_set);

};


#endif //COMPILER_GRAMMARUTILS_H