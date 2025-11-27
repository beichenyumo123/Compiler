//
// Created by 27489 on 2025/10/28.
//

#include "../LexicalAnalysis.h"

#include <iostream>
#include <fstream>
#include <vector>

#include "../../constant/Common.h"
#include "../Result.h"
using namespace std;

bool LexicalAnalysis::input(const std::string& filePath) {
    inFile.open(filePath);
    if(!inFile)
    {
        cout<<"Error"<<endl;
        return false;
    }
    inFile.get(currentChar);
    return true;
}

vector<Result> LexicalAnalysis::getAllTokens() {

    vector<Result> tokens;
    //文件指针移动到开始
    inFile.seekg(0, std::ios::beg);
    inFile.get(currentChar);
    Result result = this->nextToken();
    while (result.getTokenType()!=TOK_EOF) {

        tokens.push_back(result);
        result=this->nextToken();
    }
    return tokens;
}

Result LexicalAnalysis::nextToken()
{
    while (currentChar != '\0') {
        if (isspace(currentChar)) {
            //是否是空白字符，空格 ' ' 换行 '\n'  回车 '\r' 水平制表符 '\t'   垂直制表符 '\v'  换页 '\f'
            skipWhitespace();
            continue;
        }
        if (currentChar == '/') {  // 可能是注释或除法运算符
            skipComment();
            continue;
        }
        if (isalpha(currentChar)) {  // 标识符或关键字
            return parseIdentifier();
        }
        if (isdigit(currentChar)) {  // 数字常量
            return parseNumber();
        }
        // 运算符或分隔符
        return parseOperatorOrDelimiter();
    }
    // 输入结束
    return {TOK_EOF, ""};
}

Result LexicalAnalysis::parseOperatorOrDelimiter() {
    char current = currentChar; // 保存当前字符

        switch (current) {
            // -------------------------- 运算符 --------------------------
            case '=':
                advance(); // 移动到下一个字符
                return {TOK_ASSIGN, "="};

            case '+':
                advance();
                return {TOK_ADD, "+"};

            case '-':
                advance();
                return {TOK_SUB, "-"};

            case '*':
                advance();
                return {TOK_MUL, "*"};

            case '/':
                advance();
                return {TOK_DIV, "/"};

            case '<':
                advance(); // 先移动过 '<'
                // 检查是否为多字符运算符 "<="
                if (currentChar == '=') {
                    advance(); // 再移动过 '='
                    return {TOK_LE, "<="};
                } else {
                    // 仅为 '<'
                    return {TOK_LT, "<"};
                }

            case '>':
                advance(); // 移动过 '>'
                // 检查是否为多字符运算符 ">="
                if (currentChar == '=') {
                    advance(); // 移动过 '='
                    return {TOK_GE, ">="};
                } else {
                    // 仅为 '>'
                    return {TOK_GT, ">"};
                }

            case '!':
                advance(); // 移动过 '!'
                // 检查是否为多字符运算符 "!="
                if (currentChar == '=') {
                    advance(); // 移动过 '='
                    return {TOK_NE, "!="};
                } else {
                    // 单独的 '!' 是非法符号（根据规则无此运算符）
                    return {TOK_ERROR, "Invalid operator: !"};
                }

            // -------------------------- 分隔符 --------------------------
            case ',':
                advance();
                return {TOK_COMMA, ","};

            case ';':
                advance();
                return {TOK_SEMICOLON, ";"};

            case ':':
                advance();
                return {TOK_COLON, ":"};

            case '{':
                advance();
                return {TOK_LBRACE, "{"};

            case '}':
                advance();
                return {TOK_RBRACE, "}"};

            case '[':
                advance();
                return {TOK_LBRACKET, "["};

            case ']':
                advance();
                return {TOK_RBRACKET, "]"};

            case '(':
                advance();
                return {TOK_LPAREN, "("};

            case ')':
                advance();
                return {TOK_RPAREN, ")"};

            // 未知符号（错误处理）
            default:
                string errMsg = "未知符号: " + string(1, current);
                advance(); // 移动过错误字符，避免死循环
                return {TOK_ERROR, errMsg};
        }

}

Result LexicalAnalysis::parseNumber() {
    std::string numStr;
    bool flag = true;
    while (currentChar != '\0' && isdigit(currentChar)) {
        numStr += currentChar;
        advance();
        if (isalpha(currentChar)) {
            flag = false;
        }
    }
    if (flag) {
        return {TOK_NUM,numStr};
    }
    else {
        numStr+=currentChar;
        while (currentChar != '\0' && isalpha(currentChar)) {
            numStr += currentChar;
            advance();
        }
        string errmsg = "非法的标识符: " + numStr;
        return {TOK_ERROR,errmsg};
    }

}

Result LexicalAnalysis::parseIdentifier() {
    string id;
    while (currentChar != '\0' && isalnum(currentChar)) {
        id += currentChar;
        advance();
    }
    // 判断是否为关键字
    if (keywordMap.find(id) != keywordMap.end()) {
        return {keywordMap.at(id),id};
    }
    return {TOK_ID,id};

}

void LexicalAnalysis::skipComment()
{
    if (currentChar == '/')
    {
        advance();
        if (currentChar == '/')
        {
            while (currentChar != '\0' && currentChar != '\n')
            {
                advance();
            }
        }
        else if (currentChar == '*')
        {
            advance();  // 跳过 *
            while (currentChar != '\0')
            {
                if (currentChar == '*'
                    && inFile.peek() == '/')
                {
                    advance();  // 跳过 *
                    advance();  // 跳过 /
                    break;
                }
                advance();
            }
        }
        else {
            inFile.unget();//是除号，回退一个字符
            currentChar='/';
        }
    }
}

void LexicalAnalysis::skipWhitespace() {
    while (currentChar != '\0' && isspace(currentChar)) {
        advance();
    }
}

void LexicalAnalysis::advance() {
    if (inFile.get(currentChar)) {

    }else {
        currentChar='\0';
    }

}

void LexicalAnalysis::output() {
    // 将文件指针移动到文件开始
    inFile.seekg(0, std::ios::beg);
    inFile.get(currentChar);

    Result result = this->nextToken();
    while (result.getTokenType()!=TOK_EOF) {

        if (result.getTokenType()==TOK_ERROR) {
            cout<<"\n-----------ERROR!!!-----------"<<endl;
            result.output();
            cout<<"------------------------------\n"<<endl;
        }
        else {
            result.output();
        }

        result=this->nextToken();
    }
}

LexicalAnalysis::LexicalAnalysis(const std::string& filePath) {
    this->input(filePath);
}

