//
// Created by 27489 on 2025/11/9.
//

#include "Test.h"
#include "../class/DFA.h"
#include "../class/NFA.h"
#include "../utils/Utils.h"
#include <iostream>
#include <fstream>

//D:\Clion\Compiler\data\Contact.txt
void Test::testContact() {
    auto pair=Utils::readNFAFile(getFilePath());
    NFA* nfa=pair.first;

    NFA a=nfa[0];
    NFA b=nfa[1];

    NFA c=a+b;
    c.printNFA();
}

void Test::testUnion() {
    auto pair=Utils::readNFAFile(getFilePath());
    NFA* nfa=pair.first;

    NFA a=nfa[0];
    NFA b=nfa[1];
    NFA c=Utils::unionTwoBasicNFA(a,b);

    c.printNFA();

}

void Test::testClosure() {
    auto pair=Utils::readNFAFile(getFilePath());
    NFA* nfa=pair.first;
    NFA a=nfa[0];

    NFA b=Utils::closure(a);
    b.printNFA();
}

std::ifstream Test::getFilePath() {
    std::string path;
    std::cout<<"输入文件路径: ";
    std::cin>>path;
    std::ifstream file(path);
    if (!file) {
        std::cout<<"Open file error!"<<std::endl;
        return {};
    }
    return file;
}

DFA Test::testDetermineNFA() {
    NFA nfa1;
    nfa1.readNfaUnite(std::move(getFilePath()));
    std::cout<<"原NFA: "<<std::endl;
    nfa1.printNFA();
    DFA dfa=nfa1.determineNFA();
    std::cout<<"确定化后得到的DFA: "<<std::endl;
    dfa.printDFA();
    return dfa;
}

DFA Test::testMinimizeDFA() {
    DFA dfa=testDetermineNFA();
    DFA new_DFA=dfa.minimizeDFA();
    std::cout<<"最小化后的DFA: "<<std::endl;
    new_DFA.printDFA();
    return new_DFA;
}

NFA Test::testMergeNFA() {

    auto pair=Utils::readNFAFile(getFilePath());
    NFA* nfa_point=pair.first;
    int cnt = pair.second;

    for (int i=0;i<cnt;i++) {
        nfa_point[i].printNFA();
    }

    NFA nfa=Utils::mergeNFA(nfa_point,cnt);
    std::cout<<"合并后的NFA为: "<<std::endl;
    nfa.printNFA();
    return nfa;
}
