//
// Created by 27489 on 2025/11/11.
//

#include "../Experiment.h"

#include <iostream>

#include "../../class/RE.h"
#include "../../LexicalAnalysis/LexicalAnalysis.h"


//D:\Clion\Compiler\data\LexicalAnalysis.txt
void Experiment::experiment1() {
    std::string filePath;
    std::cout<<"----------Experiment1----------\n请输入文件路径：";
    std::cin>>filePath;
    LexicalAnalysis lexical_analysis;
    while (!lexical_analysis.input(filePath)) {
        std::cout << "请再次输入：\n";
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
    //文件第一行必须是NFA的个数
    std::string filePath;
    std::cin>>filePath;
    //auto pair=Utils::readNFAFile(std::ifstream(filePath));
    //NFA nfa=Utils::mergeNFA(pair.first,pair.second);
    //nfa.printNFA();
    NFA nfa(filePath);
    //nfa.printNFA();
    DFA dfa=nfa.determineNFA();//minimizeDFA();


    //dfa.printDFA();
    DFA d=dfa.minimizeDFA();

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
    std::cout<<"请输入正则式：";
    std::string s;
    std::cin>>s;
    RE re(s);
    NFA nfa=re.transformReToNFA();
    nfa.printNFA();
    //如果需要确定化nfa，并最小化
    /*
     *DFA dfa=nfa.determineNFA().minimizeDFA();
     *dfa.printDFA();
     */
    DFA dfa=nfa.determineNFA().minimizeDFA();
    dfa.printDFA();
    std::cout<<"\n----------Experiment3 END----------\n";
    //system("pause");
}

