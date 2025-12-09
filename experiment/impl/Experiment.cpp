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
    std::cout<<"----------Experiment1----------\n?????????·????";
    std::cin>>filePath;
    LexicalAnalysis lexical_analysis;
    while (!lexical_analysis.input(filePath)) {
        std::cout << "?????????????????????·????\n";
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
    std::cout<<"?????????·????";
    //??????????????????????NFA?????
    std::string filePath;
    std::cin>>filePath;
    //auto pair=Utils::readNFAFile(std::ifstream(filePath));
    //NFA nfa=Utils::mergeNFA(pair.first,pair.second);
    //nfa.printNFA();

    NFA nfa(filePath);
    nfa.printNFA();
    DFA dfa=nfa.determineNFA();//minimizeDFA();
    std::cout<<"----------NFA?????----------\n"<<std::endl;
    dfa.printDFA();

    //dfa.printDFA();
    DFA d=dfa.minimizeDFA();
    std::cout<<"----------DFA???С??----------\n"<<std::endl;
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
    std::cout<<"??????????????";
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

void Experiment::experiment5() {
    std::cout<<"\n----------Experiment5----------"<<std::endl;
    Grammar grammar(R"(D:\Clion\Compiler\data\GrammarTest.txt)");
    grammar.printFirstSet();
    grammar.printFollowSet();
    grammar.printSelectSet();
    //grammar.printProductionsOrder();
    std::cout<<"\n----------Experiment5 END----------\n";
}

void Experiment::experiment6() {
    std::cout<<"\n----------Experiment6----------"<<std::endl;
    Grammar grammar(R"(D:\Clion\Compiler\data\Grammar.txt)");
    grammar.printAnalysisTable();
    PredictiveAnalysis predictive_analysis(grammar);

    predictive_analysis.analysis(R"(D:\Clion\Compiler\data\SymbolString.txt)");

    std::cout<<"\n----------Experiment6 END----------\n";
}

void Experiment::experiment7() {
    std::cout<<"\n----------Experiment7----------"<<std::endl;
    Grammar grammar(R"(D:\Clion\Compiler\data\exp7.txt)");

    // grammar.printProductionsOrder();
    //grammar.printItemSet();

    DFA dfa = grammar.generateDFA();
    std::cout<<"\n----------识别活前缀的DFA----------\n";
    dfa.printDFA();
    grammar.printActionTable();
    grammar.printGoToTable();

    std::cout<<"\n----------Experiment7 END----------\n";
}

void Experiment::experiment9() {
    std::cout<<"\n----------Experiment9----------"<<std::endl;
    Grammar grammar(R"(D:\Clion\Compiler\data\LRAnalysis.txt)");
    std::string filePath=R"(D:\Clion\Compiler\data\LR_Action_GoTo.txt)";
    LRAnalysis lr_analysis(filePath,grammar);
    // lr_analysis.printActionTable();
    // lr_analysis.printGoToTable();
    std::string s="*i=i#";
    lr_analysis.analysis(s);
    std::cout<<"\n----------Experiment9 END----------\n";
}







