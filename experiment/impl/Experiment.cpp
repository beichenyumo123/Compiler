//
// Created by 27489 on 2025/11/11.
//

#include "../Experiment.h"

#include <iostream>

#include "../../class/RE.h"
#include "../../class/LexicalAnalysis.h"
#include "../../class/Grammar.h"
#include "../../class/LRAnalysis.h"
#include "../../class/PredictiveAnalysis.h"


//D:\Clion\Compiler\data\LexicalAnalysis.txt
void Experiment::experiment1() {
    std::string filePath;
    std::cout<<"----------Experiment1----------\n请输入文件路径：";
    std::cin>>filePath;
    LexicalAnalysis lexical_analysis;
    while (!lexical_analysis.input(filePath)) {
        std::cout << "文件路径错误请重新输入：\n";
        std::cin>>filePath;
    }

    lexical_analysis.output();

    std::cout<<"\n----------Experiment1 END----------\n";
    system("pause");
}

//D:\Clion\Compiler\data\NFA.txt
//D:\Clion\Compiler\data\Contact.txt
void Experiment::experiment2() {
    std::cout<<"\n----------Experiment2----------"<<std::endl;
    std::cout<<"请输入文件路径：";
    std::string filePath;
    std::cin>>filePath;
    //auto pair=Utils::readNFAFile(std::ifstream(filePath));
    //NFA nfa=Utils::mergeNFA(pair.first,pair.second);
    //nfa.printNFA();

    NFA nfa(filePath);
    std::cout<<"----------合并后的NFA----------\n"<<std::endl;
    nfa.printNFA();
    DFA dfa=nfa.determineNFA();//minimizeDFA();
    std::cout<<"----------NFA确定化----------\n"<<std::endl;
    dfa.printDFA();

    //dfa.printDFA();
    DFA d=dfa.minimizeDFA();
    std::cout<<"----------DFA最小化----------\n"<<std::endl;
    d.printDFA();
    std::cout<<"\n----------Experiment2 END----------\n";
    system("pause");
}

//a(b|c)*d
//(a|b)*c(d|e)
//ab*c*d
//(ab|cd)*e
//a(b|c)*abc
void Experiment::experiment3() {
    std::cout<<"\n----------Experiment3----------"<<std::endl;
    std::cout<<"请输入正规式：";
    std::string s;
    std::cin>>s;
    RE re(s);
    NFA nfa=re.transformReToNFA();
    std::cout<<"----------转化后的NFA----------\n"<<std::endl;
    nfa.printNFA();
    /*
     *DFA dfa=nfa.determineNFA().minimizeDFA();
     *dfa.printDFA();
     */
    std::cout<<"----------转化后的NFA确定化又最小化的结果----------\n"<<std::endl;
    DFA dfa=nfa.determineNFA().minimizeDFA();
    dfa.printDFA();
    std::cout<<"\n----------Experiment3 END----------\n";
    system("pause");
}
//D:\Clion\Compiler\data\GrammarTest.txt
void Experiment::experiment5() {
    std::cout<<"\n----------Experiment5----------"<<std::endl;
    std::cout<<"请输入文件路径：";
    std::string filePath;
    std::cin>>filePath;
    Grammar grammar(filePath);
    grammar.printFirstSet();
    grammar.printFollowSet();
    grammar.printSelectSet();
    //grammar.printProductionsOrder();
    std::cout<<"\n----------Experiment5 END----------\n";
    system("pause");
}
//D:\Clion\Compiler\data\Grammar.txt
//D:\Clion\Compiler\data\SymbolString.txt
void Experiment::experiment6() {
    std::cout<<"\n----------Experiment6----------"<<std::endl;
    std::cout<<"请输入文法文件路径：";
    std::string filePath;
    std::cin>>filePath;
    Grammar grammar(filePath);
    // grammar.printFirstSet();
    // grammar.printFollowSet();

    PredictiveAnalysis predictive_analysis(grammar);
    std::cout<<"\n请输入符号串文件路径：";
    std::string s;
    std::cin>>s;
    std::cout<<"\n----------预测分析表----------"<<std::endl;
    grammar.printAnalysisTable();
    predictive_analysis.analysis(s);

    std::cout<<"\n----------Experiment6 END----------\n";
    system("pause");
}
//D:\Clion\Compiler\data\exp7.txt
void Experiment::experiment7() {
    std::cout<<"\n----------Experiment7----------"<<std::endl;
    std::cout<<"请输入文法文件路径：";
    std::string filePath;
    std::cin>>filePath;
    Grammar grammar(filePath);
    std::cout<<"\n----------项目集族----------\n";
    grammar.printFamily();

    DFA dfa = grammar.generateDFA();
    std::cout<<"\n----------识别活前缀的DFA----------\n";
    dfa.printDFA();
    // grammar.printActionTable();
    // grammar.printGoToTable();

    grammar.writeActionAndGoToTable(filePath);

    if (grammar.is_Lr0) {
        std::cout<<"是LR0文法\n";
    }else {
        std::cout<<"不是LR0文法\n";
    }
    std::cout<<"\n----------Experiment7 END----------\n";
    system("pause");
}

//D:\Clion\Compiler\data\LRAnalysis.txt
//D:\Clion\Compiler\data\LR_Action_GoTo.txt
//D:\Clion\Compiler\data\exp9.txt
std::vector<std::string> readAllLinesFromFile(const std::string& filePath);

void Experiment::experiment9() {
    std::cout<<"\n----------Experiment9----------"<<std::endl;
    std::string grammar_path;
    std::string table_path;
    std::string symbol_string_path;
    std::cout<<"请输入文法文件的路径：";
    std::cin>>grammar_path;
    Grammar grammar(grammar_path);
    std::cout<<"请输入分析表文件的路径：";
    std::cin >> table_path;
    LRAnalysis lr_analysis(table_path,grammar);

    // lr_analysis.printActionTable();
    // lr_analysis.printGoToTable();
    std::cout<<"请输入符号串文件的路径：";
    std::cin >> symbol_string_path;
    std::vector<std::string> symbol_strings = readAllLinesFromFile(symbol_string_path);

    for (const auto& s : symbol_strings) {
        std::cout<<s<<std::endl;
        lr_analysis.analysis(s);
    }

    std::cout<<"\n----------Experiment9 END----------\n";
    system("pause");
}

// 去除字符串中所有空白字符（空格、\t、\n、\r等）
std::string removeAllWhitespace(std::string str) {
    auto newEnd = std::remove_if(str.begin(), str.end(),
        [](unsigned char c) { return std::isspace(c); });

    str.erase(newEnd, str.end());

    return str;
}

std::vector<std::string> readAllLinesFromFile(const std::string& filePath) {
    std::vector<std::string> lines;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filePath);
    }

    std::string currentLine;
    // 逐行读取，直到文件末尾
    while (std::getline(file, currentLine)) {
        lines.push_back(removeAllWhitespace(currentLine));  // 将当前行加入结果
    }

    if (file.bad()) {
        throw std::runtime_error("读取文件时发生错误: " + filePath);
    }

    file.close();
    return lines;
}







