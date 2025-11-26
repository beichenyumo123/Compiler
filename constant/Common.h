//
// Created by 27489 on 2025/11/5.
//

#ifndef COMPILER_COMON_H
#define COMPILER_COMON_H
#include <string>
#include <unordered_map>
#include <list>
#include "../enumeration/TokenType.h"

static constexpr char EPSILON='#';
static constexpr int NEW_START=-1;
static constexpr int NEW_END=-2;
static constexpr int NO_MATCH_STATE=-3;
static constexpr int NO_MATCH_SET=-4;
static constexpr int NO_STATE=-5;
static constexpr int ERROR=-6;
static constexpr int OR=-7;
static constexpr int AND=-8;
static constexpr int CLOSURE=-9;
static constexpr int LEFT_BRACKET=-10;
static constexpr int RIGHT_BRACKET=-11;
static constexpr int DEAD_STATE=-12;

struct PairHash {
    std::size_t operator()(const std::pair<int, char>& p) const {
        // 用质数31放大第一个哈希值，再与第二个哈希值相加，减少冲突
        return std::hash<int>()(p.first) * 31 + std::hash<char>()(p.second);
    }
};

using ReflectOfNFA = std::unordered_map<std::pair<int, char>, std::list<int>, PairHash>;
using ReflectOfDFA = std::unordered_map<std::pair<int,char>, int, PairHash>;

const std::string tokenTypeNames[] = {
    "",
    "TOK_MAIN",
    "TOK_IF",
    "TOK_THEN",
    "TOK_ELSE",
    "TOK_WHILE",
    "TOK_DO",
    "TOK_REPEAT",
    "TOK_UNTIL",
    "TOK_FOR",
    "TOK_FROM",
    "TOK_TO",
    "TOK_STEP",
    "TOK_SWITCH",
    "TOK_OF",
    "TOK_CASE",
    "TOK_DEFAULT",
    "TOK_RETURN",
    "TOK_INTEGER",
    "TOK_REAL",
    "TOK_CHAR",
    "TOK_BOOL",
    "TOK_AND",
    "TOK_OR",
    "TOK_NOT",
    "TOK_MOD",
    "TOK_READ",
    "TOK_WRITE",
    "TOK_ASSIGN",
    "TOK_ADD",
    "TOK_SUB",
    "TOK_MUL",
    "TOK_DIV",
    "TOK_LT",
    "TOK_LE",
    "TOK_GT",
    "TOK_GE",
    "TOK_NE",
    "TOK_COMMA",
    "TOK_SEMICOLON",
    "TOK_COLON",
    "TOK_LBRACE",
    "TOK_RBRACE",
    "TOK_LBRACKET",
    "TOK_RBRACKET",
    "TOK_LPAREN",
    "TOK_RPAREN",
    "TOK_ID",
    "TOK_NUM",
    "TOK_EOF",
    "TOK_ERROR"
};
const std::unordered_map<std::string, TokenType> keywordMap = {
    {"main", TOK_MAIN},
    {"if", TOK_IF},
    {"then", TOK_THEN},
    {"else", TOK_ELSE},
    {"while", TOK_WHILE},
    {"do", TOK_DO},
    {"repeat", TOK_REPEAT},
    {"until", TOK_UNTIL},
    {"for", TOK_FOR},
    {"from", TOK_FROM},
    {"to", TOK_TO},
    {"step", TOK_STEP},
    {"switch", TOK_SWITCH},
    {"of", TOK_OF},
    {"case", TOK_CASE},
    {"default", TOK_DEFAULT},
    {"return", TOK_RETURN},
    {"integer", TOK_INTEGER},
    {"real", TOK_REAL},
    {"char", TOK_CHAR},
    {"bool", TOK_BOOL},
    {"and", TOK_AND},
    {"or", TOK_OR},
    {"not", TOK_NOT},
    {"mod", TOK_MOD},
    {"read", TOK_READ},
    {"write", TOK_WRITE}
};

#endif //COMPILER_COMON_H