//
// Created by 27489 on 2025/11/27.
//

#ifndef COMPILER_ITEM_H
#define COMPILER_ITEM_H
#include <unordered_map>
#include <vector>

//using Item = std::unordered_map<char, std::pair<int,std::vector<char>>>
class Item {
    std::unordered_map<char, std::pair<int,std::vector<char>>> items;
};


#endif //COMPILER_ITEM_H