//
// Created by 27489 on 2025/11/18.
//

#ifndef COMPILER_GRAMMAR_H
#define COMPILER_GRAMMAR_H
#include <list>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "DFA.h"
#include "../constant/GrammarCommon.h"

//using  FirstSet = std::unordered_map<char, std::set<char>>;
//using  FollowSet = std::unordered_map<char, std::set<char>>;
//using Production = std::unordered_map<char, std::list<std::string>>;
//using SelectSet = std::unordered_map<char, std::list<std::pair<std::string, std::set<char>>>>;
//using AnalysisTable = std::unordered_map<char, std::list<std::pair<char,int >>>
//using ItemSet = unordered_set<Item>;
//using Item = std::pair<int,int>
//using Action = std::pair<char,int>;
//using ActionTable = unordered_map<int, unordered_map<char,Action>>;
//using GoToTable = unordered_map<int, unordered_map<char,int>>;
class Grammar {
public:
    char startSymbol;
    std::vector<char>terminalSymbols;
    std::vector<char> nonTerminalSymbols;
    Production productions;

    std::unordered_map<std::string,int> productions_order;
    std::unordered_map<int,std::pair<char,std::string>> production_map;


    FirstSet first_set;
    FollowSet follow_set;
    SelectSet select_set;

    AnalysisTable analysis_table;

    ItemSet items;
    std::vector<ItemSet> family;

    ActionTable action_table;
    GoToTable go_to_table;

    DFA dfa;

    explicit Grammar(const std::string& filePath);
    Grammar();
    Grammar(char startSymbol,
            std::vector<char> terminalSymbols,
            std::vector<char> nonTerminalSymbols,
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
    void printProductionsOrder();
    void printItemSet();
    void printItemSet(const ItemSet& item_set);
    void printFamily();
    void printActionTable();
    void printGoToTable();

    void writeActionAndGoToTable(const std::string& filePath);


    void generateFirstSet();
    void generateFollowSet();
    void generateSelectSet();
    void generateLLAnalysisTable();

    bool is_Lr0;
    void generateItems();
    DFA generateDFA();
    void generateActionTable();
    void generateGoToTable();


    ItemSet getEpsilonClosureOfItemSet(ItemSet item_set);
    ItemSet go(const ItemSet& I,const char& X);
    bool isTerminal(const char& c) const {
        for (char terminalSymbol : terminalSymbols) {
            if (c == terminalSymbol) return true;
        }
        return false;
    }

    bool isNonTerminal(const char& c) const {
        for (char nonTerminalSymbol : nonTerminalSymbols) {
            if (c == nonTerminalSymbol) return true;
        }
        return false;
    }
private:
    bool readGrammarFromFile(std::ifstream file);

    static bool allContainsEpsilon(FirstSet& firstSet, const std::string& right, const int& start, const int& end);
    //将一个元素的first集合中的非ε元素添加到另一个target的first集合中
    bool addNoEpsilonElementsFromFirstSet(char source, char target);

    bool addFirstSetToFollowSet(const std::string& right);

    bool checkTheNail(const char& left, const std::string& right);

    std::set<char> firstSetOfRightContainsEpsilon(const char& left, const std::string& right);

    std::set<char> getFirstAlpha(const std::string &right);

    ItemSet getJset(const ItemSet& I, const char& X);

    int findIndexInFamily(const std::vector<ItemSet>& family, const ItemSet& item_set);

    void writeLRTblFile(const std::string& filePath) const;

};


#endif //COMPILER_GRAMMAR_H