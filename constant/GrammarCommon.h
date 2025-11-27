//
// Created by 27489 on 2025/11/25.
//

#ifndef COMPILER_GRAMMARCOMON_H
#define COMPILER_GRAMMARCOMON_H

const char EPSILON_INTERNAL = '@';
const std::string EPSILON_STR = "ε";
// [关键修改] GBK 编码下的 Epsilon ("ε") 字节序列
// \xA6\xC5 是 GBK 的 "ε"
const std::string GBK_EPSILON = "\xA6\xC5";

using  FirstSet = std::unordered_map<char, std::set<char>>;
using  FollowSet = std::unordered_map<char, std::set<char>>;
using Production = std::unordered_map<char, std::list<std::string>>;
using SelectSet = std::unordered_map<char, std::list<std::pair<std::string, std::set<char>>>>;
using AnalysisTable = std::unordered_map<char, std::unordered_map<char,int>>;

#endif //COMPILER_GRAMMARCOMON_H