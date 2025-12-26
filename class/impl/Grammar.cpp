//
// Created by 27489 on 2025/11/18.
//

#include "../Grammar.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stack>

#include "../../constant/GrammarCommon.h"
using namespace std;
//using Item = std::unordered_map<char, std::unordered_map<int,int>>
void Grammar::generateItems() {
    if (!items.empty()) return;
    for (const auto& production : productions) {
        auto left = production.first;
        auto rights = production.second;
        for (auto right : rights) {
            string p = string(1, left) + " -> " + right;
            if (right[0] == EPSILON_INTERNAL) {
                items.insert(make_pair(productions_order[p], 1));
            }else {
                for (int i=0;i<=right.length();i++) {
                    items.insert(make_pair(productions_order[p], i));
                }
            }
        }
    }
}


void Grammar::generateFirstSet() {
     if (!first_set.empty()) return;
    for (char c:terminalSymbols) {
        first_set[c]={c};
    }
    bool flag=true;
    while (flag) {
        flag=false;
        for (const auto& production : productions) {
            char left=production.first;
            for (string right : production.second) {
                //?????????????????
                if (isTerminal(right[0]) || right[0] == EPSILON_INTERNAL) {
                    if (first_set.count(left)!=0) {
                        //??????
                        if (first_set[left].insert(right[0]).second) {
                            flag = true;
                        }
                    }
                    else {
                        flag=true;
                        first_set[left]={right[0]};
                    }
                }
                //??????????????????
                else {
                    //??????????????????first????????????????first????
                    flag = addNoEpsilonElementsFromFirstSet(right[0],left);
                    //????????????
                    if (allContainsEpsilon(first_set, right,0,right.length()-1)) {
                        if (first_set[left].insert(EPSILON_INTERNAL).second) {
                            flag = true;
                        }
                    }

                    int length=right.length();
                    for (int i=1;i<length;i++) {
                        if (allContainsEpsilon(first_set,right,0,i)) {
                            if (addNoEpsilonElementsFromFirstSet(right[i],left)) {
                                flag = true;
                            }
                        }
                    }

                }
            }
        }
    }
}

void Grammar::generateFollowSet() {
    if (!follow_set.empty()) return;
    follow_set[startSymbol]={'#'};
    bool flag=true;
    while (flag) {
        flag = false;
        for (const auto& production : productions) {
            auto left = production.first;
            auto rights = production.second;
            for (const auto& right : rights) {
                if (addFirstSetToFollowSet(right)) {
                    flag=true;
                }
                if (checkTheNail(left, right)) {
                    flag=true;
                }
            }
        }
    }

}

//using SelectSet = std::unordered_map<char, std::list<std::pair<std::string, std::set<char>>>>;
void Grammar::generateSelectSet() {
    for (auto production : productions) {
        auto left = production.first;
        auto rights = production.second;
        for (auto right : rights) {
            auto res=firstSetOfRightContainsEpsilon(left,right);
            if (select_set.count(left) == 0) {
                select_set[left] = {make_pair(right,res)};
            }else {
                select_set[left].emplace_back(right,res);
            }
        }
    }
}

//using AnalysisTable = std::unordered_map<char, std::list<std::pair<char,int >>>
void Grammar::generateLLAnalysisTable() {
    for (auto production : productions) {
        auto left = production.first;
        auto rights = production.second;
        for (auto right : rights) {
            string p = string(1,left) + " -> " + right;

            auto first_alpha = getFirstAlpha(right);
            for (auto a : first_alpha) {
                if (a == EPSILON_INTERNAL){
                    for (auto b : follow_set[left]) {
                        analysis_table[left][b] = productions_order[p];
                    }
                    continue;
                }
                analysis_table[left][a] = productions_order[p];
            }
        }

    }
}


set<char> Grammar::getFirstAlpha(const std::string &right) {
    if (right[0] == EPSILON_INTERNAL) {
        return {EPSILON_INTERNAL};
    }
    set<char> firstAlpha;

    for (auto c : first_set[right[0]]) {
        if (c != EPSILON_INTERNAL) firstAlpha.insert(c);
    }

    for (int j=1;j<right.length();j++) {
        if (allContainsEpsilon(first_set,right,0,j-1)) {
            for (auto c : first_set[right[j]]) {
                if (c != EPSILON_INTERNAL) firstAlpha.insert(c);
            }
        }else {
            break;
        }
    }

    if (allContainsEpsilon(first_set,right,0,right.size()-1)) {
        firstAlpha.insert(EPSILON_INTERNAL);
    }

    return firstAlpha;
}

set<char> Grammar::firstSetOfRightContainsEpsilon(const char& left,const std::string &right) {
    if (right[0] == EPSILON_INTERNAL) {
        return  follow_set[left];
    }
    set<char> firstAlpha = getFirstAlpha(right);

    if (firstAlpha.count(EPSILON_INTERNAL) == 0) {
        return firstAlpha;
    }else {
        set<char> result = follow_set[left];
        for (char c : firstAlpha) {
            if (c!=EPSILON_INTERNAL) {
                result.insert(c);
            }
        }
        return result;
    }
}

//FollowSet????????
bool addFollowAToFollowB(FollowSet& follow_set, const char& source, const char& target) {
    bool flag=false;
    if (follow_set.count(source)!=0) {
        for (auto i : follow_set[source]) {
            if (follow_set[target].insert(i).second) {
                flag = true;
            }
        }
    }

    return flag;
}

bool Grammar::checkTheNail(const char &left, const std::string &right) {
    int n=right.length()-1;
    bool flag=false;
    //nonTerminalSymbols.count(right[n])!=0
    if (isNonTerminal(right[n])) {
        if (addFollowAToFollowB(follow_set,left,right[n])) {
            flag = true;
        }
    }
    for (int i=0;i<n;i++) {
        //terminalSymbols.count(right[i])!=0
        if (isTerminal(right[i])) {
            continue;
        }

        if (allContainsEpsilon(first_set,right,i+1,n)) {
            if (addFollowAToFollowB(follow_set,left,right[i])) {
                flag=true;
            }
        }
    }

    return flag;
}
//??first???????????????????????follow??????
bool addNonEpsilonElementsToFollowSet(const char& source,const char& target,FirstSet& first_set, FollowSet& follow_set) {
    bool flag = false;
    for (auto i : first_set[source]) {
        if (i!=EPSILON_INTERNAL) {
            if (follow_set[target].insert(i).second) {
                flag = true;
            }
        }

    }
    return flag;
}

bool Grammar::addFirstSetToFollowSet(const std::string &right) {
    bool flag=false;
    int n=right.length();
    for (int j=1;j<n;j++) {
        int i=j-1;
        //nonTerminalSymbols.count(right[i])!=0
        if (isNonTerminal(right[i])) {
            if (addNonEpsilonElementsToFollowSet(right[j],right[i],first_set,follow_set)) {
                flag = true;
            }
        }
    }
    return flag;
}

//FirstSet????????
bool Grammar::addNoEpsilonElementsFromFirstSet(char source, char target) {
    //???????????????
    bool flag=false;
    if (first_set.count(source)!=0) {
        for (char c : first_set[source]) {
            if (first_set[target].insert(c).second) {
                flag=true;
            }
        }
    }
    return flag;
}

bool Grammar::allContainsEpsilon(FirstSet &firstSet, const std::string &right, const int &start, const int &end) {
    for (int i=start;i<=end;i++) {
        if (firstSet.count(right[i])!=0) {
            if (firstSet[right[i]].count(EPSILON_INTERNAL)==0) {
                return false;
            }
        }
        else {
            return false;
        }
    }
    return true;
}

Grammar::Grammar(const std::string& filePath) {
    ifstream file(filePath);
    if (!file) {
        cout<<"Open File Error!"<<endl;
        return;
    }

    if (!readGrammarFromFile(std::move(file))) {
        cout<<"ERROR!\n";
        Grammar();
        return;
    }
    generateFirstSet();
    generateFollowSet();
    generateSelectSet();
    generateLLAnalysisTable();
    generateItems();
}

Grammar::Grammar() {
    startSymbol='\0';
    terminalSymbols.clear();
    nonTerminalSymbols.clear();
    productions.clear();
    first_set.clear();
    follow_set.clear();
    analysis_table.clear();
}

//???? T' -> t ????????
static std::string normalizeString(const std::string& raw) {
    std::string noSpaces;
    // 1. ??????
    for (char c : raw) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            noSpaces += c;
        }
    }

    // ??????????????????????? "??"??????????????????
    // 2. [??????] ??? GBK ?? Epsilon
    if (noSpaces == GBK_EPSILON) {
        // ??????????????????????????
        // std::cout << "[Debug] Found GBK Epsilon!" << std::endl;
        return std::string(1, EPSILON_INTERNAL);
    }

    // 2. ???? T' -> t
    std::string processed;
    for (size_t i = 0; i < noSpaces.length(); ++i) {
        // ??? UTF-8 ??????????????????????????????????????? ASCII??
        // ???????????????? ?? ?????????? char????????????
        // ?????????????? ?? ????????????????????????? if ??????????

        // ??? T'
        if (i + 1 < noSpaces.length() && noSpaces[i + 1] == '\'') {
            processed += std::tolower(static_cast<unsigned char>(noSpaces[i]));
            i++; // ???? '
        } else {
            processed += noSpaces[i];
        }
    }
    return processed;
}

static char normalizeChar(const std::string& raw) {
    std::string s = normalizeString(raw);
    if (s.empty()) return '\0'; // ??????
    return s[0];
}

bool Grammar::readGrammarFromFile(ifstream infile) {
    try {
        // 1. ??????
        int n; infile >> n;
        for (int i = 0; i < n; ++i) {
            std::string temp; infile >> temp;
            nonTerminalSymbols.push_back(normalizeChar(temp));
        }

        // 2. ????
        int m; infile >> m;
        for (int i = 0; i < m; ++i) {
            std::string temp; infile >> temp;
            // ????????????????? ???????? (???????????????????????????????????????)
            if (temp == EPSILON_STR) {
                // ??? ?? ?????? terminalSymbols???????????????? token
                // ???????????????? insert(EPSILON_INTERNAL);
            } else {
                terminalSymbols.push_back(temp[0]);
            }
        }

        // 3. ?????
        int k; infile >> k;
        std::string dummy; std::getline(infile, dummy); // ???????

        for (int i = 0; i < k; ++i) {
            std::string line;
            std::getline(infile, line);
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string rawLhs, arrow, rawRhs;
            ss >> rawLhs >> arrow;
            std::getline(ss, rawRhs); // ????????

            char lhs = normalizeChar(rawLhs);
            std::string rhs = normalizeString(rawRhs); // ?????? "??" ??? "@"

            productions[lhs].push_back(rhs);

            string production = string(1, lhs) + " -> " + rhs;
            productions_order[production]=i;
            production_map[i] = make_pair(lhs,rhs);
        }

        // 4. ???????
        std::string rawStart; infile >> rawStart;
        startSymbol = normalizeChar(rawStart);

    } catch (...) { return false; }
    return true;
}


void Grammar::printGrammar() {
    cout<<"????????"<<startSymbol<<endl;
    cout<<"????????";
    for (const char& nt : nonTerminalSymbols) {
        cout<<nt<<" ";
    }
    cout<<endl;

    cout<<"??????";
    for (const char& t : terminalSymbols) {
        cout<<t<<" ";
    }
    cout<<endl;

    cout<<"???????"<<endl;
    for (const auto& i: productions) {
        const auto left = i.first;
        auto rights = i.second;
        for (const std::string& right : rights) {
            cout<<left<<" -> "<<right<<endl;
        }
    }
}

void Grammar::printFirstSet() {
    if (first_set.empty()) {
        generateFirstSet();
    }
    cout<<"----------FirstSet----------\n";
    for (const auto& i : first_set) {
        cout<<i.first<<": ";
        for (auto j : i.second) {
            if (j=='@') {
                cout<<"??"<<" ";
            }
            else {
                cout<<j<<" ";
            }

        }
        cout<<endl;
    }
}

void Grammar::printFollowSet() {
    if (follow_set.empty()) {
        generateFollowSet();
    }
    cout<<"----------FollowSet----------\n";
    for (const auto& i : follow_set) {
        cout<<i.first<<": ";
        for (auto j : i.second) {
            if (j=='@') {
                cout<<"??"<<" ";
            }
            else {
                cout<<j<<" ";
            }
        }
        cout<<endl;
    }
}
//using SelectSet = std::unordered_map<char, std::list<std::pair<std::string, std::set<char>>>>;
void Grammar::printSelectSet() {
    if (select_set.empty()) {
        generateSelectSet();
    }
    cout<<"----------SelectSet----------\n";
    for (auto i : select_set) {
        auto left=i.first;
        for (auto j : i.second) {
            cout<<left<<" -> ";
            if (j.first[0]==EPSILON_INTERNAL) {
                cout<<"??: ";
            }else {
                cout<<j.first<<": ";
            }

            for (auto k : j.second) {
                if (k == EPSILON_INTERNAL) {
                    cout<<"??";
                }else {
                    cout<<k<<" ";
                }
            }
            cout<<endl;
        }
    }
}
//using AnalysisTable = std::unordered_map<char, std::unordered_map<char,int>>
void Grammar::printAnalysisTable() {
    int columnWidth = 8;

    if (terminalSymbols.empty()) {
        std::cerr << "Error" << std::endl;
        return;
    }
    if (analysis_table.empty()) {
        return;
    }


    int totalLineLen = columnWidth + terminalSymbols.size() * columnWidth;

    std::cout << std::string(totalLineLen, '-') << std::endl;


    std::cout << std::left << std::setw(columnWidth) << " ";

    for (char colSymbol : terminalSymbols) {
        std::cout << std::left << std::setw(columnWidth) << colSymbol;
    }
    std::cout << std::endl;


    std::cout << std::string(totalLineLen, '-') << std::endl;


    std::vector<char> rowKeys;
    for (const auto& row : analysis_table) {
        rowKeys.push_back(row.first);
    }
    std::sort(rowKeys.begin(), rowKeys.end());


    for (char rowKey : rowKeys) {

        std::cout << std::left << std::setw(columnWidth) << rowKey;


        const auto& cellMap = analysis_table.at(rowKey);


        for (char colSymbol : terminalSymbols) {
            auto it = cellMap.find(colSymbol);
            if (it != cellMap.end()) {

                std::string valueStr = "p" + std::to_string(it->second);
                std::cout << std::left << std::setw(columnWidth) << valueStr;
            } else {

                std::cout << std::left << std::setw(columnWidth) << " ";
            }
        }


        std::cout << std::endl;
    }

    std::cout << std::string(totalLineLen, '-') << std::endl;
}

void Grammar::printProductionsOrder() {
    for (auto s : productions_order) {
        cout<<s.second<<": "<<s.first<<endl;
    }
}

void Grammar::printItemSet() {
    this->printItemSet(this->items);
}

void Grammar::printItemSet(const ItemSet& item_set) {
    for (auto i : item_set) {
        auto left = production_map[i.first].first;
        auto right = production_map[i.first].second;
        cout<<"\t"<<left<<"->";
        if (right[0] == EPSILON_INTERNAL) {
            cout<<".\n";
        }else {
            int j=0;
            for (;j<right.length();j++) {
                if (j == i.second) {
                    cout<<".";
                }
                cout<<right[j];
            }
            if (j == i.second) {
                cout<<".";
            }
            cout<<endl;
        }
    }
}

void Grammar::printFamily() {

    for (int i=0;i<family.size();i++) {
        cout<<"I"<<i<<": \n";
        this->printItemSet(family[i]);
    }
}

DFA Grammar::generateDFA() {
    int order = productions_order[std::string(1,startSymbol) + " -> " + *(productions[startSymbol].begin())];
    family.push_back(getEpsilonClosureOfItemSet({std::make_pair(order,0)}));

    std::set<int> states={0};//??????
    std::set<char> chars(terminalSymbols.begin(),terminalSymbols.end());//?????
    chars.insert(nonTerminalSymbols.begin(),nonTerminalSymbols.end());
    ReflectOfDFA transitions;//???
    int original_state = 0; //??????
    std::set<int> final_states;


    int i=0;
    while (i < family.size()) {
        for (char X : chars) {
            auto  set = go(family[i],X);

            if (!set.empty()) {
                int next=-1;
                int index=findIndexInFamily(family,set);
                if (index == -1) {
                    family.push_back(set);
                    next = family.size()-1;
                }
                else {
                    next = index;
                }
                transitions[{i,X}]= next;
            }
        }
        i++;
        if (i < family.size()) states.insert(i);
    }

    printFamily();

    this->dfa = {states,chars,transitions,original_state,final_states};
    return dfa;
}
//using Action = std::pair<char,int>;
//using ActionTable = unordered_map<int, unordered_map<char,Action>>;

void Grammar::generateActionTable() {
    if (dfa.empty()) {
        generateDFA();
    }
    is_Lr0 = true;
    for (int i=0;i<family.size();i++) {
        bool r = false;
        bool s = false;
        for (auto item : family[i]) {
            auto right = production_map[item.first].second;
            //??????
            if (item.second == right.length()) {
                if (production_map[item.first].first == startSymbol) {
                    action_table[i]['#'] = make_pair('a',-1);
                }
                else {
                    if (r || s) {
                        is_Lr0=false;
                        return;
                    }
                    r = true;
                    for (auto c : terminalSymbols) {
                        action_table[i][c] = make_pair('r',item.first);
                    }
                    action_table[i]['#'] = make_pair('r',item.first);
                }
            }else {
                if (r) {
                    is_Lr0 = false;
                    return;
                }
                s = true;
                if (isTerminal(right[item.second])) {
                    int j = dfa.transitions[{i,right[item.second]}];
                    action_table[i][right[item.second]] = make_pair('s',j);
                }
            }
        }
    }
}
//using Action = std::pair<char,int>;
//using ActionTable = unordered_map<int, unordered_map<char,Action>>;
void Grammar::printActionTable() {
    if (action_table.empty()) {
        generateActionTable();
    }
    cout<<"\n----------ActionTable----------\n";
    for (const auto& i : action_table) {
        for (const auto j : i.second) {
            cout<<"("<<i.first<<", "<<j.first<<")"<<": ";
            if (j.second.first == 'a') {
                cout<<"acc"<<endl;
                continue;
            }
            cout<<j.second.first<<j.second.second<<endl;
        }
    }

}

//using GoToTable = unordered_map<int, unordered_map<char,int>>;
void Grammar::generateGoToTable() {
    if (dfa.empty()) {
        generateDFA();
    }
    for (auto transition : dfa.transitions) {
        //nonTerminalSymbols.count(transition.first.second)
        if (isNonTerminal(transition.first.second)) {
            go_to_table[transition.first.first][transition.first.second] = transition.second;
        }
    }
}
void Grammar::printGoToTable() {
    if (go_to_table.empty()) {
        generateGoToTable();
    }
    cout<<"\n----------GoTo Table----------\n";
    for (auto i : go_to_table) {
        for (auto j : i.second) {
            cout<<"("<<i.first<<", "<<j.first<<")"<<": "<<j.second<<endl;
        }
    }
}


int Grammar::findIndexInFamily(const std::vector<ItemSet>& family,const ItemSet &item_set) {
    auto it =  std::find(family.begin(), family.end(), item_set);
    if (it != family.end()) {
        return  std::distance(family.begin(), it);
    }
    return -1;
}


ItemSet Grammar::go(const ItemSet& I, const char& X) {
    return getEpsilonClosureOfItemSet(getJset(I,X));
}

ItemSet Grammar::getJset(const ItemSet &I, const char &X) {
    ItemSet res;
    for (auto i : I) {
        auto right = production_map[i.first].second;

        if (i.second+1 <= right.size()) {
            if (X == right[i.second]) {
                res.insert(make_pair(i.first,i.second+1));
            }
        }
    }

    return res;
}


ItemSet Grammar::getEpsilonClosureOfItemSet(ItemSet item_set) {
    ItemSet res = item_set;
    bool flag = false;
    while (!flag) {
        flag = true;
        for (auto i : item_set) {
            string right = production_map[i.first].second;
            int k = i.second;
            if (k<right.length()) {
                if (isNonTerminal(right[k])) {
                    //??????????????????????
                    stack<char> s;
                    s.push(right[k]);
                    while (!s.empty()) {
                        char current = s.top();
                        s.pop();
                        for (const auto& production : productions[current]) {
                            string p = string(1,current) + " -> " + production;
                            int order = productions_order[p];
                            if (res.insert(make_pair(order,0)).second) {
                                if (isNonTerminal(production_map[order].second[0])) {
                                    s.push(production_map[order].second[0]);
                                }
                                flag = false;
                            }
                        }
                    }

                }
            }
        }
    }

    return res;
}

void Grammar:: writeLRTblFile(const std::string& filename) const {
    // 打开文件（覆盖写入，不存在则创建）
    std::ofstream out_file(filename, std::ios::out | std::ios::trunc);
    if (!out_file.is_open()) {
        throw std::runtime_error("无法打开文件进行写入: " + filename);
    }

    // ========== 第一步：统计Action表总项数 ==========
    size_t action_count = 0;
    for (const auto& state_entry : action_table) {
        // 累加每个状态下的终结符-动作对数量
        action_count += state_entry.second.size();
    }

    // ========== 写入Action表项数 + Action表内容 ==========
    // 先写入Action表项数（格式：Action表项数: 数字）
    out_file << action_count << "\n";
    for (const auto& state_entry : action_table) {
        int state = state_entry.first;                // 外层key：状态号
        const auto& symbol_action_map = state_entry.second;  // 内层：终结符->Action

        for (const auto& symbol_action : symbol_action_map) {
            char symbol = symbol_action.first;        // 终结符（如'(', 'a', '#'）
            const Action& action = symbol_action.second;  // 动作对

            // 解析 Action 为 lrtbl 格式的字符串
            std::string action_str;
            if (action.first == 'a' && action.second == -1) {
                // {a,-1} 对应 acc（接受）
                action_str = "acc";
            } else if (action.first == 's') {
                // {s, n} 对应 sn（移进至状态n）
                action_str = "s" + std::to_string(action.second);
            } else if (action.first == 'r') {
                // {r, n} 对应 rn（归约产生式n）
                action_str = "r" + std::to_string(action.second);
            } else {
                // 未知动作类型，抛出异常提示
                throw std::runtime_error(
                    "未知的Action类型: 状态" + std::to_string(state) +
                    " 符号'" + symbol + "' 动作(" + action.first + "," +
                    std::to_string(action.second) + ")");
            }

            // 按 lrtbl 格式写入：状态 符号 动作
            out_file << state << " " << symbol << " " << action_str << "\n";
        }
    }

    // ========== 第二步：统计GoTo表总项数 ==========
    size_t goto_count = 0;
    for (const auto& state_entry : go_to_table) {
        // 累加每个状态下的非终结符-转移状态对数量
        goto_count += state_entry.second.size();
    }

    // ========== 写入GoTo表项数 + GoTo表内容 ==========
    // 先写入GoTo表项数（格式：GoTo表项数: 数字）
    out_file << goto_count << "\n";
    for (const auto& state_entry : go_to_table) {
        int state = state_entry.first;                // 外层key：状态号
        const auto& symbol_nextstate_map = state_entry.second;  // 内层：非终结符->转移状态

        for (const auto& symbol_nextstate : symbol_nextstate_map) {
            char symbol = symbol_nextstate.first;     // 非终结符（如'S', 'E'）
            int next_state = symbol_nextstate.second; // 转移后的状态

            // 按 lrtbl 格式写入：状态 符号 转移状态
            out_file << state << " " << symbol << " " << next_state << "\n";
        }
    }

    // 检查写入是否成功
    if (out_file.bad()) {
        out_file.close();
        throw std::runtime_error("文件写入过程中发生错误: " + filename);
    }

    // 关闭文件
    out_file.close();
    std::cout << "lrtbl 文件生成成功：" << filename << std::endl;
}

// 替换文件路径的扩展名为.lrtbl
std::string replaceExtensionToLrtbl(const std::string& filePath) {
    // 找到最后一个目录分隔符的位置（处理 / 和 \ 两种分隔符）
    size_t lastSeparator = filePath.find_last_of("/\\");
    // 找到最后一个 . 的位置
    size_t lastDot = filePath.find_last_of('.');

    // 确保 . 出现在最后一个目录分隔符之后（是文件名的扩展名，而非目录名中的 .）
    if (lastDot != std::string::npos && lastDot > lastSeparator) {
        // 截断到 . 的位置，拼接 .lrtbl
        return filePath.substr(0, lastDot) + ".lrtbl";
    } else {
        // 没有找到有效的扩展名，直接在末尾添加 .lrtbl
        return filePath + ".lrtbl";
    }
}

void Grammar::writeActionAndGoToTable(const std::string& filePath){
    if (action_table.empty()) {
        generateActionTable();
    }
    if (go_to_table.empty()) {
        generateGoToTable();
    }
    writeLRTblFile(replaceExtensionToLrtbl(filePath));
}
