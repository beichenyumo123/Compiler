//
// Created by 27489 on 2025/11/13.
//

#ifndef COMPILER_TOKENTYPE_H
#define COMPILER_TOKENTYPE_H

enum TokenType {
    // -------------------------- 关键字（Keyword）--------------------------
    TOK_MAIN=1,         // main
    TOK_IF,           // if
    TOK_THEN,         // then
    TOK_ELSE,         // else
    TOK_WHILE,        // while
    TOK_DO,           // do
    TOK_REPEAT,       // repeat
    TOK_UNTIL,        // until
    TOK_FOR,          // for
    TOK_FROM,         // from
    TOK_TO,           // to
    TOK_STEP,         // step
    TOK_SWITCH,       // switch
    TOK_OF,           // of
    TOK_CASE,         // case
    TOK_DEFAULT,      // default
    TOK_RETURN,       // return
    TOK_INTEGER,      // integer
    TOK_REAL,         // real
    TOK_CHAR,         // char
    TOK_BOOL,         // bool
    TOK_AND,          // and
    TOK_OR,           // or
    TOK_NOT,          // not
    TOK_MOD,          // mod
    TOK_READ,         // read
    TOK_WRITE,        // write

    // -------------------------- 运算符（Operator）--------------------------
    TOK_ASSIGN,       // =（赋值/等于，根据语法规则区分，词法阶段统一为该类型）
    TOK_ADD,          // +
    TOK_SUB,          // -
    TOK_MUL,          // *
    TOK_DIV,          // /
    TOK_LT,           // <（小于）
    TOK_LE,           // <=（小于等于）
    TOK_GT,           // >（大于）
    TOK_GE,           // >=（大于等于）
    TOK_NE,           // !=（不等于）
    // 'ε'
    // -------------------------- 分隔符（Delimiter）--------------------------
    TOK_COMMA,        // ,（逗号）
    TOK_SEMICOLON,    // ;（分号）
    TOK_COLON,        // :（冒号）
    TOK_LBRACE,       // {（左花括号）
    TOK_RBRACE,       // }（右花括号）
    TOK_LBRACKET,     // [（左方括号）
    TOK_RBRACKET,     // ]（右方括号）
    TOK_LPAREN,       // (（左圆括号）
    TOK_RPAREN,       // )（右圆括号）

    // -------------------------- 其他标记 --------------------------
    TOK_ID,           // 标识符（ID，符合 letter(letter|digit)* 规则）
    TOK_NUM,          // 数字（NUM，符合 digit+ 规则）

    // -------------------------- 特殊标记 --------------------------
    TOK_EOF,          // 输入结束（End of File）
    TOK_ERROR         // 错误标记（非法字符或不符合规则的标记）
};

#endif //COMPILER_TOKENTYPE_H