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
    //引入新的初态结点X和终态结点Y
    transitions[{NEW_START,EPSILON}] = {*original_state.begin()};
    transitions[{*final_states.begin(),EPSILON}] = {NEW_END};

    //s作为状态转移矩阵的行
    vector<set<int>> s;
    //初始化首行首列元素
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
    //生成状态转移矩阵
    while (i < (int)s.size()) {
        //对于每一列，产生第i行某列的元素
        for (char c : chars) {
            //拿到Ic
            set<int> t = Utils::getEpsilonClosureOfSetAfterGetNextByCharOfSet(s[i], c, transitions);

            if (t.empty()) continue;
            //是否在第一列中出现过
            auto it = find(s.begin(), s.end(), t);
            //target记录其在第一列的行数
            int target;
            //不存在
            if (it == s.end()) {
                //插入到现在的最后一行，并记录编号
                target = s.size();
                s.push_back(t);
                //查找是否含有原终止状态，并标记
                bool f=false;
                for (int j:final_states) {
                    if (t.count(j)!=0) {
                        f=true;
                    }
                }
                isFinal.push_back(f);
            }
            //已存在记录其行号
            else {
                target = it - s.begin();
            }
            //重新编号建立映射关系
            new_transition[{i, c}] = target;
        }
        //生成状态转移矩阵的下一行
        i++;
    }

    // 构建DFA
    set<int> new_states;
    set<int> finals;
    //标记新的终止状态
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
    cout << "状态集: ";
    for (int s : states) cout << s << " ";
    cout << "\n字母表: ";
    for (char c : chars) cout << c << " ";
    cout << "\n映射: " << endl;
    for (const auto& entry : transitions) {
        cout << "(" << entry.first.first << ", '" << entry.first.second << "') -> ";
        for (int to : entry.second) cout << to << " ";
        cout << endl;
    }
    cout << "初态: ";
    for (int f : original_state) cout << f << " ";
    cout<< "\n终态: ";
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
        cout<<"?????????\n";
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
            //?????????????????????????????
            file.seekg(0, std::ios::beg);
            auto pair=Utils::readNFAFile(std::move(file));
            // for (int i=0;i<pair.second;i++) {
            //     pair.first[i].printNFA();
            // }
            return Utils::mergeNFA(pair.first,pair.second);
        }
        else {
            NFA temp; // ???????
            temp.readNfaUnite(std::move(file)); // ??????????NFA?????
            return temp; // ???????????
        }
    }
}

bool readNonEmptyLine(std::ifstream& file, std::string& line) {
    while (std::getline(file, line)) {
        size_t start = line.find_first_not_of(" \t");
        size_t end = line.find_last_not_of(" \t");
        if (start != std::string::npos) {
            line = line.substr(start, end - start + 1);
            return true;
        }
    }
    return false;
}

ifstream NFA::readNfaUnite(std::ifstream file) {
    if (!file) {
        cout<<"????????";
        return {};
    }
    string line;
    // 1. ?????????????????У?
    if (!readNonEmptyLine(file, line)) {
        cerr << "???????????????" << endl;
        return {};
    }
    istringstream iss1(line);
    int num;
    while (iss1 >> num) {
        states.insert(num);
    }

    // 2. ??????????????????У?
    if (!readNonEmptyLine(file, line)) { // ?滻?getline(file, line)
        cerr << "????????????????" << endl;
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

    // 3. ??????????????n?????????У?
    if (!readNonEmptyLine(file, line)) { // ?滻?getline(file, line)
        cerr << "????????????????????" << endl;
        return {};
    }
    int n = stoi(line);

    // 4. ???n?????????????????????У?
    for (int i = 0; i < n; ++i) {
        if (!readNonEmptyLine(file, line)) { // ?滻?getline(file, line)
            cerr << "????????" << i << "????????????" << endl;
            return {};
        }
        istringstream iss3(line);

        // ????from_state
        int from_state;
        if (!(iss3 >> from_state)) {
            cerr << "??????????from_state" << endl;
            return {};
        }

        // ????symbol
        char symbol;
        iss3 >> ws;
        int next = iss3.peek();
        if (next == EOF) {
            cerr << "??????????????" << endl;
            return {};
        }
        if (next == '-' || isdigit(next)) {
            int char_flag;
            if (!(iss3 >> char_flag)) {
                cerr << "?????????????" << endl;
                return {};
            }
            symbol = (char_flag == -1) ? '#' : ('0' + char_flag);
        } else {
            if (!(iss3 >> symbol)) {
                cerr << "????????????" << endl;
                return {};
            }
        }

        // ????to_states
        list<int> to_states;
        int to_state;
        while (iss3 >> to_state) {
            to_states.push_back(to_state);
        }

        transitions[{from_state, symbol}] = to_states;
    }

    // 5. ?????????????????У?
    if (!readNonEmptyLine(file, line)) { // ?滻?getline(file, line)
        cerr << "??????????????" << endl;
        return {};
    }
    istringstream iss4(line);
    while (iss4 >> num) {
        original_state.insert(num);
    }

    // 6. ?????????????????У?
    if (!readNonEmptyLine(file, line)) { // ?滻?getline(file, line)
        cerr << "??????????????" << endl;
        return {};
    }
    istringstream iss5(line);
    while (iss5 >> num) {
        final_states.insert(num);
    }

    return file;
}




