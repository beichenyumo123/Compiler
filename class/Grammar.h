//
// Created by 27489 on 2025/11/18.
//

#ifndef COMPILER_GRAMMAR_H
#define COMPILER_GRAMMAR_H
#include <list>
#include <set>
#include <string>
#include <unordered_map>

#include "../constant/GrammarCommon.h"

//using  FirstSet = std::unordered_map<char, std::set<char>>;
//using  FollowSet = std::unordered_map<char, std::set<char>>;
//using Production = std::unordered_map<char, std::list<std::string>>;
//using SelectSet = std::unordered_map<char, std::list<std::pair<std::string, std::set<char>>>>;
//using AnalysisTable = std::unordered_map<char, std::list<std::pair<char,std::string >>>
class Grammar {
public:
    char startSymbol;
    std::set<char> terminalSymbols;
    std::set<char> nonTerminalSymbols;
    Production productions;

    FirstSet first_set;
    FollowSet follow_set;
    SelectSet select_set;

    AnalysisTable analysis_table;

    explicit Grammar(const std::string& filePath);
    Grammar();
    Grammar(char startSymbol,
            std::set<char> terminalSymbols,
            std::set<char> nonTerminalSymbols,
            Production productions)
            : startSymbol(startSymbol),
              terminalSymbols(std::move(terminalSymbols)),
              nonTerminalSymbols(std::move(nonTerminalSymbols)),
              productions(std::move(productions)) {}

    void printGrammar();
    void printFirstSet();
    void printFollowSet();
    void printSelectSet();
    void printAnalysisTable();

    void generateFirstSet();
    void generateFollowSet();
    void generateSelectSet();
    void generateAnalysisTable();
private:
    bool readGrammarFromFile(std::ifstream file);

    static bool allContainsEpsilon(FirstSet& firstSet, const std::string& right, const int& start, const int& end);
    //将一个元素的first集合中的非ε元素添加到另一个target的first集合中
    bool addNoEpsilonElementsFromFirstSet(char source, char target);

    bool addFirstSetToFollowSet(const std::string& right);

    bool checkTheNail(const char& left, const std::string& right);

    std::set<char> firstSetOfRightContainsEpsilon(const char& left, const std::string& right);

    std::set<char> getFirstAlpha(const char& left,const std::string& right);

};


#endif //COMPILER_GRAMMAR_H