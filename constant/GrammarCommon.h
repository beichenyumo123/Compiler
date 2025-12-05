//
// Created by 27489 on 2025/11/25.
//

#ifndef COMPILER_GRAMMARCOMON_H
#define COMPILER_GRAMMARCOMON_H
#include <algorithm>
#include <utility>       // for std::pair
#include <functional>    // for std::hash
#include <unordered_set> // for std::unordered_set

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
using Item = std::pair<int,int>;

// 特化 std::hash<Item>
namespace std {
    template <>
    struct hash<Item> {
        size_t operator()(const Item& item) const noexcept {
            // 获取 first 和 second 的哈希值
            size_t h1 = std::hash<int>{}(item.first);
            size_t h2 = std::hash<int>{}(item.second);

            // Hash Combine 算法 (类似 boost::hash_combine)
            // 0x9e3779b9 是黄金分割率的近似值，用于打散位模式，减少冲突
            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }
    };
}

using ItemSet = std::unordered_set<Item>;
using Action = std::pair<char,int>;
using ActionTable = std::unordered_map<int, std::unordered_map<char,Action>>;
using GoToTable = std::unordered_map<int, std::unordered_map<char,int>>;


#endif //COMPILER_GRAMMARCOMON_H