cmake_minimum_required(VERSION 4.0)
project(Compiler)

set(CMAKE_CXX_STANDARD 11)

add_executable(Compiler main.cpp
        class/impl/LexicalAnalysis.cpp
        class/LexicalAnalysis.h
        class/impl/Result.cpp
        class/Result.h
        class/impl/NFA.cpp
        class/NFA.h
        class/impl/DFA.cpp
        class/DFA.h
        utils/impl/Utils.cpp
        utils/Utils.h
        constant/Common.h
        test/Test.cpp
        test/Test.h
        experiment/impl/Experiment.cpp
        experiment/experiment.h
        class/impl/RE.cpp
        class/RE.h
        enumeration/TokenType.h
        class/impl/Grammar.cpp
        class/Grammar.h
        utils/impl/GrammarUtils.cpp
        utils/GrammarUtils.h
        constant/GrammarCommon.h
        class/impl/PredictiveAnalysis.cpp
        class/PredictiveAnalysis.h
        class/impl/Item.cpp
        class/Item.h
        class/impl/LRAnalysis.cpp
        class/LRAnalysis.h
)
