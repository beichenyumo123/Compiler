//
// Created by 27489 on 2025/11/26.
//

#include "../PredictiveAnalysis.h"

#include <algorithm>
#include <fstream>
#include <iostream>
using namespace std;

bool readLineWithoutWhitespace(std::ifstream& fin, std::string& out_line);

void PredictiveAnalysis::analysis(const std::string &file_path) {

    std::ifstream file(file_path);
    if (!file) {
        cout<<"Open File Error!\n";
        return;
    }

    std::string input_string;

    if (!readLineWithoutWhitespace(file, input_string)) {
        cout<<"Read File Error!\n";
        return;
    }
    input_string += '#';
    cout<<endl<<input_string<<"分析过程：\n";

    char c = input_string[0];
    int index=0;

    while (!analysis_stack.empty()) {
        analysis_stack.pop();
    }

    cout<<"初始化：#入栈，S入栈；\n";
    analysis_stack.push('#');
    analysis_stack.push(grammar.startSymbol);


    bool flag=true;
    int cnt=1;
    while (flag) {
        cout<<cnt++<<":";
        char x = analysis_stack.top();
        analysis_stack.pop();
        cout<<"出栈X="<<x<<",  输入c="<<c<<", ";
        //grammar.terminalSymbols.count(x) != 0
        if (grammar.isTerminal(x)) {
            if (x == c) {
                cout<<"匹配, 输入指针后移\n";
                index++;
                if (index == input_string.length()) {
                    cout<<"ERROR!"<<endl;
                    return;
                }
                c=input_string[index];
            }
        }
        else if (x == '#') {
            if (x == c) {
                cout<<"匹配，成功。"<<endl;
                flag = false;
            }
            else {
                cout<<"ERROR!"<<endl;
                return;
            }
        }
        else if (grammar.analysis_table[x].count(c)) {
            string right = grammar.production_map[grammar.analysis_table[x][c]].second;
            cout<<"查表: M[X,c] = "<<x<<" -> ";
            if (right[0] != EPSILON_INTERNAL) {
                for (int i=right.length()-1;i>=0;i--) {
                    analysis_stack.push(right[i]);
                }
                cout<<right<<", 产生式右部逆序入栈; \n";
            }
            else {
                // TODO
                cout<<"\n";
            }
        }else {
            cout<<"ERROR!"<<endl;
            return;
        }

    }


}

bool readLineWithoutWhitespace(std::ifstream& fin, std::string& out_line) {
    // 1. ??????У????????п???
    std::string raw_line;
    if (!std::getline(fin, raw_line)) {
        return false; // ???????β/?????????????
    }

    // 2. ???????п?????????????\t??\r??\n ???
    // remove_if????????????????????棬???????β??????
    auto new_end = std::remove_if(raw_line.begin(), raw_line.end(),
        [](char c) {
            // isspace???ж????????????????????п???????
            return std::isspace(static_cast<unsigned char>(c));
        });
    // erase??????β????Ч???????????????棬?????????
    raw_line.erase(new_end, raw_line.end());

    // 3. ???????????
    out_line = raw_line;
    return true;
}
