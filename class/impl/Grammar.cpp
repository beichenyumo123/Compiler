//
// Created by 27489 on 2025/11/18.
//

#include "../Grammar.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../../constant/GrammarCommon.h"
using namespace std;


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
                //如果右部第一个是终结符
                if (terminalSymbols.count(right[0])!=0 || right[0] == EPSILON_INTERNAL) {
                    if (first_set.count(left)!=0) {
                        if (first_set[left].insert(right[0]).second) {
                            flag = true;
                        }
                    }
                    else {
                        flag=true;
                        first_set[left]={right[0]};
                    }
                }
                //如果右部第一个是非终结符
                else {
                    //将右部第一个非终结符的first集合加入左部非终结符的first集合
                    flag = addNoEpsilonElementsFromFirstSet(right[0],left);
                    //是否全部包含ε
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

//using AnalysisTable = std::unordered_map<char, std::list<std::pair<char,std::string >>>
void Grammar::generateAnalysisTable() {
    for (auto production : productions) {
        auto left = production.first;
        auto rights = production.second;
        for (auto right : rights) {
            if (right[0] == EPSILON_INTERNAL) {
                for (auto b : follow_set[left]) {
                    analysis_table[left].emplace_back(make_pair(b,right));
                }
                continue;
            }
            auto first_alpha = getFirstAlpha(left,right);
            for (auto a : first_alpha) {
                if (a == EPSILON_INTERNAL){
                    for (auto b : follow_set[left]) {
                        analysis_table[left].emplace_back(make_pair(b,right));
                    }
                    continue;
                }
                analysis_table[left].emplace_back(make_pair(a,right));
            }
        }

    }
}


set<char> Grammar::getFirstAlpha(const char& left,const std::string &right) {
    set<char> firstAlpha;

    if ( first_set[right[0]].count(EPSILON_INTERNAL) == 0) {
        firstAlpha=first_set[right[0]];
    }
    else {
        for (int j=1;j<right.length();j++) {
            if (allContainsEpsilon(first_set,right,0,j-1)) {
                for (auto c : first_set[right[j]]) {
                    firstAlpha.insert(c);
                }
            }else {
                break;
            }

        }
    }

    return firstAlpha;
}

set<char> Grammar::firstSetOfRightContainsEpsilon(const char& left,const std::string &right) {
    if (right[0] == EPSILON_INTERNAL) {
        return  follow_set[left];
    }
    set<char> firstAlpha = getFirstAlpha(left,right);

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

//FollowSet辅助函数
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
    if (nonTerminalSymbols.count(right[n])!=0) {
        if (addFollowAToFollowB(follow_set,left,right[n])) {
            flag = true;
        }
    }
    for (int i=0;i<n;i++) {
        if (terminalSymbols.count(right[i])!=0) {
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
//将first集合中的非ε元素添加到相映的follow集合中
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
        if (nonTerminalSymbols.count(right[i])!=0) {
            if (addNonEpsilonElementsToFollowSet(right[j],right[i],first_set,follow_set)) {
                flag = true;
            }
        }
    }
    return flag;
}

//FirstSet辅助函数
bool Grammar::addNoEpsilonElementsFromFirstSet(char source, char target) {
    //返回值表示是否有变化
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
    generateAnalysisTable();
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

//处理 T' -> t 并移除空格
static std::string normalizeString(const std::string& raw) {
    std::string noSpaces;
    // 1. 移除空格
    for (char c : raw) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            noSpaces += c;
        }
    }

    // 特殊判断：如果整个右部就是 "ε"，直接返回对应的内部字符
    // 2. [关键修复] 匹配 GBK 的 Epsilon
    if (noSpaces == GBK_EPSILON) {
        // 调试确认：一旦匹配成功，这行会打印
        // std::cout << "[Debug] Found GBK Epsilon!" << std::endl;
        return std::string(1, EPSILON_INTERNAL);
    }

    // 2. 处理 T' -> t
    std::string processed;
    for (size_t i = 0; i < noSpaces.length(); ++i) {
        // 检查 UTF-8 字符边界（防止错误地拆分多字节字符，虽然这里主要处理 ASCII）
        // 但如果你的输入流把 ε 读成了两个 char，这里比较棘手。
        // 简单起见，我们假设 ε 作为单独的产生式右部出现，上面 if 已经处理了。

        // 检查 T'
        if (i + 1 < noSpaces.length() && noSpaces[i + 1] == '\'') {
            processed += std::tolower(static_cast<unsigned char>(noSpaces[i]));
            i++; // 跳过 '
        } else {
            processed += noSpaces[i];
        }
    }
    return processed;
}

static char normalizeChar(const std::string& raw) {
    std::string s = normalizeString(raw);
    if (s.empty()) return '\0'; // 异常保护
    return s[0];
}

bool Grammar::readGrammarFromFile(ifstream infile) {
    try {
        // 1. 非终结符
        int n; infile >> n;
        for (int i = 0; i < n; ++i) {
            std::string temp; infile >> temp;
            nonTerminalSymbols.insert(normalizeChar(temp));
        }

        // 2. 终结符
        int m; infile >> m;
        for (int i = 0; i < m; ++i) {
            std::string temp; infile >> temp;
            // 如果终结符列表中包含 ε，也要映射 (一般终结符列表不包含空串，但在某些定义中可能)
            if (temp == EPSILON_STR) {
                // 通常 ε 不存入 terminalSymbols，因为它不是真实的 token
                // 但如果你需要，可以 insert(EPSILON_INTERNAL);
            } else {
                terminalSymbols.insert(temp[0]);
            }
        }

        // 3. 产生式
        int k; infile >> k;
        std::string dummy; std::getline(infile, dummy); // 吃掉换行

        for (int i = 0; i < k; ++i) {
            std::string line;
            std::getline(infile, line);
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string rawLhs, arrow, rawRhs;
            ss >> rawLhs >> arrow;
            std::getline(ss, rawRhs); // 读取剩余部分

            char lhs = normalizeChar(rawLhs);
            std::string rhs = normalizeString(rawRhs); // 这里会把 "ε" 变成 "@"

            productions[lhs].push_back(rhs);
        }

        // 4. 开始符号
        std::string rawStart; infile >> rawStart;
        startSymbol = normalizeChar(rawStart);

    } catch (...) { return false; }
    return true;
}


void Grammar::printGrammar() {
    cout<<"开始符号："<<startSymbol<<endl;
    cout<<"非终结符：";
    for (const char& nt : nonTerminalSymbols) {
        cout<<nt<<" ";
    }
    cout<<endl;

    cout<<"终结符：";
    for (const char& t : terminalSymbols) {
        cout<<t<<" ";
    }
    cout<<endl;

    cout<<"产生式："<<endl;
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
    for (auto i : first_set) {
        cout<<i.first<<": ";
        for (auto j : i.second) {
            if (j=='@') {
                cout<<"ε"<<" ";
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
    for (auto i : follow_set) {
        cout<<i.first<<": ";
        for (auto j : i.second) {
            if (j=='@') {
                cout<<"ε"<<" ";
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
                cout<<"ε: ";
            }else {
                cout<<j.first<<": ";
            }

            for (auto k : j.second) {
                if (k == EPSILON_INTERNAL) {
                    cout<<"ε ";
                }else {
                    cout<<k<<" ";
                }
            }
            cout<<endl;
        }
    }
}
//using AnalysisTable = std::unordered_map<char, std::list<std::pair<char,std::string >>>
void Grammar::printAnalysisTable() {
    for (auto i : analysis_table) {
        for (auto j : i.second) {
            cout<<"("<<i.first<<","<<j.first<<"): "<<i.first<<"->";
            if (j.second[0] == EPSILON_INTERNAL) {
                cout<<"ε\n";
            }
            else {
                cout<<j.second<<endl;
            }

        }
    }
}

