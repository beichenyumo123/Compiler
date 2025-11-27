//
// Created by 27489 on 2025/11/11.
//

#include "../Experiment.h"

#include <iostream>

#include "../../class/RE.h"
#include "../../class/LexicalAnalysis.h"
#include "../../class/Grammar.h"
#include "../../class/PredictiveAnalysis.h"


//D:\Clion\Compiler\data\LexicalAnalysis.txt
void Experiment::experiment1() {
    std::string filePath;
    std::cout<<"----------Experiment1----------\n请输入文件路径：";
    std::cin>>filePath;
    LexicalAnalysis lexical_analysis;
    while (!lexical_analysis.input(filePath)) {
        std::cout << "文件打开失败，请再次输入文件路径：\n";
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
    //文件内容的第一行需要是包含的NFA的个数
    std::string filePath;
    std::cin>>filePath;
    //auto pair=Utils::readNFAFile(std::ifstream(filePath));
    //NFA nfa=Utils::mergeNFA(pair.first,pair.second);
    //nfa.printNFA();

    NFA nfa(filePath);
    nfa.printNFA();
    DFA dfa=nfa.determineNFA();//minimizeDFA();
    std::cout<<"----------NFA确定化----------\n"<<std::endl;
    dfa.printDFA();

    //dfa.printDFA();
    DFA d=dfa.minimizeDFA();
    std::cout<<"----------DFA?最小化----------\n"<<std::endl;
    d.printDFA();
    std::cout<<"\n----------Experiment2 END----------\n";
    //system("pause");
}

//a(b|c)*d
//(a|b)*c(d|e)
//ab*c*d
//(ab|cd)*e
//a(b|c)*abc
void Experiment::experiment3() {
    std::cout<<"\n----------Experiment3----------"<<std::endl;
    std::cout<<"请入正则表达式：";
    std::string s;
    std::cin>>s;
    RE re(s);
    NFA nfa=re.transformReToNFA();
    nfa.printNFA();
    /*
     *DFA dfa=nfa.determineNFA().minimizeDFA();
     *dfa.printDFA();
     */
    DFA dfa=nfa.determineNFA().minimizeDFA();
    dfa.printDFA();
    std::cout<<"\n----------Experiment3 END----------\n";
    //system("pause");
}

void Experiment::experiment4() {
    std::cout<<"\n----------Experiment4----------"<<std::endl;
    Grammar grammar(R"(D:\Clion\Compiler\data\GrammarTest.txt)");
    // grammar.printFirstSet();
    // grammar.printFollowSet();
    // grammar.printSelectSet();
    grammar.printProductionsOrder();
    std::cout<<"\n----------Experiment4 END----------\n";
}

void Experiment::experiment5() {
    std::cout<<"\n----------Experiment5----------"<<std::endl;
    Grammar grammar(R"(D:\Clion\Compiler\data\Grammar.txt)");
    grammar.printAnalysisTable();
    PredictiveAnalysis predictive_analysis(grammar);

    predictive_analysis.analysis(R"(D:\Clion\Compiler\data\SymbolString.txt)");

    std::cout<<"\n----------Experiment5 END----------\n";
}



