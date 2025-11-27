//
// Created by 27489 on 2025/10/28.
//

#ifndef COMPILER_LEXICALANALYSIS_H
#define COMPILER_LEXICALANALYSIS_H

#include <string>
#include <fstream>
#include <vector>

#include "Result.h"

class LexicalAnalysis {

    // std::string sourceCode;
    // std::pair<int,int> p;
    char currentChar{};//当前字符
    std:: ifstream inFile;

public:
    explicit LexicalAnalysis(const std::string& filePath);
    LexicalAnalysis()=default;

    Result nextToken();//获取下一个token
    bool input(const std::string& filePath);//读取，打开文件
    std::vector<Result> getAllTokens();
    void output();//输出
private:

    void advance();             //获取下一个字符
    void skipWhitespace();      //跳过空白字符如空格、制表符等
    void skipComment();         //跳过注释
    Result parseIdentifier();   //处理标识符
    Result parseNumber();       //处理数字
    Result parseOperatorOrDelimiter();//处理操作符或分隔符
};


#endif //COMPILER_LEXICALANALYSIS_H

