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
    // 逐行读取并过滤空白
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
        cout<<"出栈X="<<x<<", 输入c="<<c<<", ";
        if (grammar.terminalSymbols.count(x) != 0) {
            if (x == c) {
                cout<<"匹配, 输入指针往后移\n";
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
                cout<<"匹配成功"<<endl;
                flag = false;
            }
            else {
                cout<<"ERROR!"<<endl;
                return;
            }
        }
        else if (grammar.analysis_table[x].count(c)) {
            string right = grammar.right_map[grammar.analysis_table[x][c]];
            cout<<"查表: M[X,c] = "<<x<<" -> ";
            if (right[0] != EPSILON_INTERNAL) {
                for (int i=right.length()-1;i>=0;i--) {
                    analysis_stack.push(right[i]);
                }
                cout<<right<<", 产生式右部逆序入栈; \n";
            }
            else {
                cout<<"ε\n";
            }
        }else {
            cout<<"ERROR!"<<endl;
            return;
        }

    }


}

bool readLineWithoutWhitespace(std::ifstream& fin, std::string& out_line) {
    // 1. 读取整行（包含所有空白）
    std::string raw_line;
    if (!std::getline(fin, raw_line)) {
        return false; // 读到文件尾/出错，返回失败
    }

    // 2. 过滤所有空白字符（移除空格、\t、\r、\n 等）
    // remove_if：把非空白字符移到字符串前面，返回新的尾迭代器
    auto new_end = std::remove_if(raw_line.begin(), raw_line.end(),
        [](char c) {
            // isspace：判断是否是空白字符（包含所有空白类型）
            return std::isspace(static_cast<unsigned char>(c));
        });
    // erase：删除末尾的无效字符（空白被移到后面，需要擦除）
    raw_line.erase(new_end, raw_line.end());

    // 3. 输出过滤后的结果
    out_line = raw_line;
    return true;
}
