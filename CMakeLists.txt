cmake_minimum_required(VERSION 4.0)
project(Compiler)

set(CMAKE_CXX_STANDARD 11)

add_executable(Compiler main.cpp
        LexicalAnalysis/impl/LexicalAnalysis.cpp
        LexicalAnalysis/LexicalAnalysis.h
        class/impl/Result.cpp
        class/Result.h
        class/impl/NFA.cpp
        class/NFA.h
        class/impl/DFA.cpp
        class/DFA.h
        utils/impl/Utils.cpp
        utils/Utils.h
        constant/Comon.h
        test/Test.cpp
        test/Test.h
        experiment/impl/Experiment.cpp
        experiment/experiment.h
        class/impl/RE.cpp
        class/RE.h
        enumeration/TokenType.h
)
