//
// Created by 27489 on 2025/10/29.
//

#include "../NFA.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <set>
#include <limits>
#include <algorithm>

#include "../../utils/Utils.h"
using namespace std;


DFA NFA::determineNFA() {
    //引入X初始状态，Y初始状态
    transitions[{NEW_START,EPSILON}] = {*original_state.begin()};
    transitions[{*final_states.begin(),EPSILON}] = {NEW_END};

    //s代表状态转移矩阵的第一列
    vector<set<int>> s;
    //初始化第一行第一列
    s.push_back(Utils::getEpsilonClosureOfSet({NEW_START}, transitions));
    vector<bool> isFinal;
    bool q=false;
    for (int j:final_states) {
        if (s[0].count(j)!=0) {
            q=true;
        }
    }
    isFinal.push_back(q);

    unordered_map<pair<int,char>, int, PairHash> new_transition;
    int i = 0;
    //构造状态转移矩阵，i表示某行，s的行数会逐渐递增到最后不变，循环会停止
    while (i < (int)s.size()) {
        for (char c : chars) {
            set<int> t = Utils::getEpsilonClosureOfSetAfterGetNextByCharOfSet(s[i], c, transitions);
            //忽略空集
            if (t.empty()) continue;
            //新的集合是否在状态转移矩阵的第一列
            auto it = find(s.begin(), s.end(), t);
            //target相当于在构造状态转移矩阵时逐渐递增的新编号
            int target;
            //新的集合未在第一列出现
            if (it == s.end()) {
                //新的集合编号为当前状态转移矩阵的行数（从0开始编号，不需要+1）
                target = s.size();
                //新集合加入到状态转移矩阵的首列
                s.push_back(t);
                //判断这个集合中是否包含原来的终止状态并标记
                bool f=false;
                for (int j:final_states) {
                    if (t.count(j)!=0) {
                        f=true;
                    }
                }
                isFinal.push_back(f);
            }
            //在第一列出现
            else {
                target = it - s.begin();
            }
            //建立响应的映射关系，某行首列状态借助字符c转换到target状态
            new_transition[{i, c}] = target;
        }
        //开始下一行
        i++;
    }

    // 组装 DFA
    set<int> new_states;
    set<int> finals;
    //初始化新的终止状态集合
    for (int j = 0; j < (int)s.size(); j++) {
        new_states.insert(j);
        if (isFinal[j]) finals.insert(j);
    }
    return {new_states, chars, new_transition, 0, finals};
}

NFA NFA::operator+(const NFA &a) const{

    set<char> new_chars=this->chars;
    for (char c: a.chars) {
        new_chars.insert(c);
    }

    int next_state=*this->final_states.rbegin()+1;
    // if (next_state ==*a.original_state.begin()) {
    //     next_state=0;
    // }

    unordered_map<int,int> map;
    for (int i : a.states) {
        map[i]=next_state;
        next_state++;
    }

    set<int> new_states=this->states;
    for (int i:a.states) {
        new_states.insert(map[i]);
    }
    ReflectOfNFA new_transitions=this->transitions;
    for (const auto& i: a.transitions) {
        auto j=i.first;

        list<int> l;
        for (int k:i.second) {
            l.push_back(map[k]);
        }
        new_transitions[{map[j.first],j.second}]=l;
    }
    new_transitions[{*final_states.rbegin(),EPSILON}]={map[*a.original_state.begin()]};

    set<int> new_final_states;
    for (int i: a.final_states) {
        new_final_states.insert(map[i]);
    }

    return {new_states, new_chars, new_transitions,original_state,new_final_states};
}

void NFA::printNFA() const {
    cout << "状态集合: ";
    for (int s : states) cout << s << " ";
    cout << "\n符号集合: ";
    for (char c : chars) cout << c << " ";
    cout << "\n转换规则: " << endl;
    for (const auto& entry : transitions) {
        cout << "(" << entry.first.first << ", '" << entry.first.second << "') → ";
        for (int to : entry.second) cout << to << " ";
        cout << endl;
    }
    cout << "初始状态: ";
    for (int f : original_state) cout << f << " ";
    cout<< "\n终止状态: ";
    for (int f : final_states) cout << f << " ";
    cout << endl<<endl;
}

NFA::NFA(){
    this->states = {};
    this->final_states = {};
    this->transitions = {};
    this->original_state = {};
    this->final_states = {};
}

NFA::NFA(std::set<int> states, std::set<char> chars, ReflectOfNFA transitions, std::set<int> original_state) {
    this->states=std::move(states);
    this->chars=std::move(chars);
    this->transitions=std::move(transitions);
    this->original_state=std::move(original_state);
    this->final_states={};
}

NFA::NFA(std::set<int> states, std::set<char> chars, ReflectOfNFA transitions, std::set<int> original_state, std::set<int> final_states) {
    this->states = std::move(states);
    this->chars = std::move(chars);
    this->transitions = std::move(transitions);
    this->original_state = std::move(original_state);
    this->final_states = std::move(final_states);
}

NFA NFA::buildFromFile(const std::string &filePath) {
    ifstream file(filePath);
    if (!file) {
        cout<<"初始化错误\n";
        return {};
    }else {
        int n;
        file >> n;
        if (n <= 0) {
            std::cout << "Invalid n" << std::endl;
            return {};
        }
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (n>1) {
            //文件指针移动到最开始，确保读取的一致性
            file.seekg(0, std::ios::beg);
            auto pair=Utils::readNFAFile(std::move(file));
            // for (int i=0;i<pair.second;i++) {
            //     pair.first[i].printNFA();
            // }
            return Utils::mergeNFA(pair.first,pair.second);
        }
        else {
            NFA temp; // 临时对象
            temp.readNfaUnite(std::move(file)); // 调用读取单个NFA的逻辑
            return temp; // 返回临时对象
        }
    }
}

// 辅助函数：从文件流中读取下一个非空行（跳过空白行）
// 返回值：是否成功读取到非空行；line参数存储读取到的非空行
bool readNonEmptyLine(std::ifstream& file, std::string& line) {
    while (std::getline(file, line)) {
        // 去除行首尾的空白字符（空格、制表符等）
        size_t start = line.find_first_not_of(" \t");
        size_t end = line.find_last_not_of(" \t");
        if (start != std::string::npos) { // 存在非空白字符
            line = line.substr(start, end - start + 1); // 截取有效部分
            return true;
        }
        // 空行：继续读取下一行
    }
    // 文件结束或读取失败
    return false;
}

ifstream NFA::readNfaUnite(std::ifstream file) {
    if (!file) {
        cout<<"文件打开错误";
        return {};
    }
    string line;
    // 1. 读取状态集合（跳过空行）
    if (!readNonEmptyLine(file, line)) {
        cerr << "错误：读取状态集合失败" << endl;
        return {};
    }
    istringstream iss1(line);
    int num;
    while (iss1 >> num) {
        states.insert(num);
    }

    // 2. 读取字符集合（跳过空行）
    if (!readNonEmptyLine(file, line)) { // 替换原getline(file, line)
        cerr << "错误：读取字符集合失败" << endl;
        return {};
    }
    istringstream iss2(line);
    int sym_num;
    char c;
    while (iss2) {
        int next = iss2.peek();
        if (next == EOF) break;
        if (isdigit(next)) {
            if (iss2 >> sym_num) {
                chars.insert('0' + sym_num);
            }
        } else {
            if (iss2 >> c) {
                chars.insert(c);
            }
        }
    }

    // 3. 读取转换规则数量n（跳过空行）
    if (!readNonEmptyLine(file, line)) { // 替换原getline(file, line)
        cerr << "错误：读取转换规则数量失败" << endl;
        return {};
    }
    int n = stoi(line);

    // 4. 读取n条转换规则（每条都跳过空行）
    for (int i = 0; i < n; ++i) {
        if (!readNonEmptyLine(file, line)) { // 替换原getline(file, line)
            cerr << "错误：读取第" << i << "条转换规则失败" << endl;
            return {};
        }
        istringstream iss3(line);

        // 解析from_state
        int from_state;
        if (!(iss3 >> from_state)) {
            cerr << "错误：无法读取from_state" << endl;
            return {};
        }

        // 解析symbol
        char symbol;
        iss3 >> ws;
        int next = iss3.peek();
        if (next == EOF) {
            cerr << "错误：缺少第二个元素" << endl;
            return {};
        }
        if (next == '-' || isdigit(next)) {
            int char_flag;
            if (!(iss3 >> char_flag)) {
                cerr << "错误：读取整数失败" << endl;
                return {};
            }
            symbol = (char_flag == -1) ? '#' : ('0' + char_flag);
        } else {
            if (!(iss3 >> symbol)) {
                cerr << "错误：读取字符失败" << endl;
                return {};
            }
        }

        // 解析to_states
        list<int> to_states;
        int to_state;
        while (iss3 >> to_state) {
            to_states.push_back(to_state);
        }

        transitions[{from_state, symbol}] = to_states;
    }

    // 5. 读取初始状态（跳过空行）
    if (!readNonEmptyLine(file, line)) { // 替换原getline(file, line)
        cerr << "错误：读取初始状态失败" << endl;
        return {};
    }
    istringstream iss4(line);
    while (iss4 >> num) {
        original_state.insert(num);
    }

    // 6. 读取终止状态（跳过空行）
    if (!readNonEmptyLine(file, line)) { // 替换原getline(file, line)
        cerr << "错误：读取终止状态失败" << endl;
        return {};
    }
    istringstream iss5(line);
    while (iss5 >> num) {
        final_states.insert(num);
    }

    return file;
}




