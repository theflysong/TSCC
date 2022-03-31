#pragma once

#include <vector>
#include <string>

enum class TokenType {
    EMPTY = 0,
    NUMBER,
    IDENTIFIER,
    COMMA,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    COLON,
    STRING,
    ASSIGN,
    ADD,
    SUB,
    MUL,
    SMUL,
    DIV,
    SDIV,
    REM,
    SREM,
    AND,
    OR,
    XOR,
    NOT,
    SHL,
    ASHR,
    LSHR,
    NEG,
    CALL,
    WITH,
    RET,
    BR,
    GOTO,
    CZ,
    CNZ,
    CEQ,
    CNE,
    CGT,
    CGTS,
    CGE,
    CGES,
    CLT,
    CLTS,
    CLE,
    CLES,
    LEA,
    TYPE_BYTE,
    TYPE_WORD,
    TYPE_DWORD,
    TYPE_QWORD,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_POINTER,
    VARIABLES,
    DOT,
    COMMENT,
    NEWLINE,
    LABEL,
    DEFINE,
    NOP,
    SELECT,
    ERROR = -1
};

std::string to_string(TokenType type);

struct Token {
    TokenType type;
    std::string content;
};

std::vector<Token> lex(std::string input);
void init_lexer(void);