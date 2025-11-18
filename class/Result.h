//
// Created by 27489 on 2025/10/28.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H
#include <string>
#include "../enumeration/TokenType.h"

#include <unordered_map>



class Result {
    TokenType syn;
    std::string token;
    //int sum=-1;
public:
    //Result(TokenType syn, const std::string& token,int sum);
    Result(TokenType syn, const std::string& token);
    void output() const;
    TokenType getTokenType() const;

};


#endif //COMPILER_TOKEN_H