#include "tscc_lexer.h"
#include <string.h>
#include <iostream>

//other: 0, [0-9]: 1, [a-z][A-Z]'_': 2, ',': 3, '[': 4, ']': 5,  ':':6
//'"':7 '\':8 eof:9 '.': 10 '#': 11 \n:12
int char_mapping[128] = {};

int trans[][13] = {
//       0   1   2   3   4   5   6   7   8   9  10  11  12
/* 0 */{-2,  1,  2,  3,  4,  5,  6,  7, -1, -2, 10, 11, 12},
/* 1 */{-2,  1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2},
/* 2 */{-2,  2,  2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2},
/* 3 */{-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2},
/* 4 */{-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2},
/* 5 */{-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2},
/* 6 */{-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2},
/* 7 */{ 7,  7,  7,  7,  7,  7,  7,  9,  8, -1, -2, -2, -2},
/* 8 */{ 7,  7,  7,  7,  7,  7,  7,  7,  7, -1, -2, -2, -2},
/* 9 */{-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2},
/*10 */{-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2},
/*11 */{11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, -2},
/*12 */{-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}
};

std::pair<int, int> run_once(std::string str, const int start) {
    int state = 0;
    int last_state = 0;
    int position = start;
    while (state >= 0) {
        last_state = state;
        if (position >= str.length())
            state = trans[state][9] > 0 ? -1 : trans[state][9];
        else
            state = trans[state][char_mapping[str.at(position)]];
        position ++;
    }
    if (state == -1) last_state = -1;
    return std::make_pair<>(last_state, position);
}

TokenType state2type[] = {TokenType::EMPTY, TokenType::NUMBER, TokenType::IDENTIFIER, TokenType::COMMA,
TokenType::LEFT_BRACKET, TokenType::RIGHT_BRACKET, TokenType::COLON, TokenType::ERROR, TokenType::ERROR,
TokenType::STRING, TokenType::DOT, TokenType::COMMENT, TokenType::NEWLINE};

std::string to_string(TokenType type) {
    switch (type)
    {
    case TokenType::EMPTY:
        return "empty";
    case TokenType::NUMBER:
        return "number";
    case TokenType::IDENTIFIER:
        return "identifier";
    case TokenType::COMMA:
        return "comma";
    case TokenType::LEFT_BRACKET:
        return "left_bracket";
    case TokenType::RIGHT_BRACKET:
        return "right_bracket";
    case TokenType::COLON:
        return "colon";
    case TokenType::STRING:
        return "string";
    case TokenType::ASSIGN:
        return "assign";
    case TokenType::ADD:
        return "add";
    case TokenType::SUB:
        return "sub";
    case TokenType::MUL:
        return "mul";
    case TokenType::SMUL:
        return "smul";
    case TokenType::DIV:
        return "div";
    case TokenType::SDIV:
        return "sdiv";
    case TokenType::REM:
        return "rem";
    case TokenType::SREM:
        return "srem";
    case TokenType::AND:
        return "and";
    case TokenType::OR:
        return "or";
    case TokenType::XOR:
        return "xor";
    case TokenType::NOT:
        return "not";
    case TokenType::SHL:
        return "shl";
    case TokenType::ASHR:
        return "ashr";
    case TokenType::LSHR:
        return "lshr";
    case TokenType::NEG:
        return "neg";
    case TokenType::CALL:
        return "call";
    case TokenType::WITH:
        return "with";
    case TokenType::RET:
        return "ret";
    case TokenType::BR:
        return "br";
    case TokenType::GOTO:
        return "goto";
    case TokenType::CZ:
        return "cz";
    case TokenType::CNZ:
        return "cnz";
    case TokenType::CEQ:
        return "ceq";
    case TokenType::CNE:
        return "cne";
    case TokenType::CGT:
        return "cgt";
    case TokenType::CGTS:
        return "cgts";
    case TokenType::CGE:
        return "cge";
    case TokenType::CGES:
        return "cges";
    case TokenType::CLT:
        return "clt";
    case TokenType::CLTS:
        return "clts";
    case TokenType::CLE:
        return "cle";
    case TokenType::CLES:
        return "cles";
    case TokenType::LEA:
        return "lea";
    case TokenType::TYPE_BYTE:
        return "byte";
    case TokenType::TYPE_WORD:
        return "word";
    case TokenType::TYPE_DWORD:
        return "dword";
    case TokenType::TYPE_QWORD:
        return "qword";
    case TokenType::TYPE_FLOAT:
        return "float";
    case TokenType::TYPE_DOUBLE:
        return "double";
    case TokenType::TYPE_POINTER:
        return "pointer";
    case TokenType::VARIABLES:
        return "variables";
    case TokenType::DOT:
        return "dot";
    case TokenType::NEWLINE:
        return "newline";
    case TokenType::COMMENT:
        return "comment";
    case TokenType::LABEL:
        return "label";
    case TokenType::DEFINE:
        return "define";
    case TokenType::NOP:
        return "nop";
    case TokenType::SELECT:
        return "select";
    default:
        return "error";
    }
}

class Trie {
    struct TrieNode {
        TrieNode *children[64];
        TokenType type;
        TrieNode(TokenType type = TokenType::EMPTY) {
            memset(children, 0, sizeof(children));
            this->type = type;
        }
        ~TrieNode() {
            for (auto child : children)
                if (child != NULL)
                    delete child;
        }
    };
    TrieNode roots[64];
    static int char2num(char ch) {
        if (islower(ch)) return ch - 'a' + 1;
        if (isupper(ch)) return ch - 'A' + 27;
        if (isdigit(ch)) return ch - '0' + 53;
        return 0;
    }
public:
    Trie() {}
    ~Trie() {}
    void insert(std::string str, TokenType type) {
        if (str.length() == 0)
            return;
        int i = 0;
        TrieNode *nd = &roots[char2num(str.at(i ++))];
        while (i < str.length()) {
            if (nd->children[char2num(str.at(i))] == NULL)
                nd->children[char2num(str.at(i))] = new TrieNode();
            nd = nd->children[char2num(str.at(i ++))];
        }
        nd->type = type;
    }
    TokenType lookup(std::string str) {
        if (str.length() == 0)
            return TokenType::EMPTY;
        int i = 0;
        TrieNode *nd = &roots[char2num(str.at(i ++))];
        while (i < str.length()) {
            if (nd->children[char2num(str.at(i))] == NULL)
                return TokenType::EMPTY;
            nd = nd->children[char2num(str.at(i ++))];
        }
        return nd->type;
    }
};

Trie keywords;

std::vector<Token> lex(std::string input) {
    int position = 0;
    std::vector<Token> tokens;
    while (position < input.length()) {
        auto res = run_once(input, position);
        if (res.first == -1) {
            std::cerr << "[Error]Invalid Input" << std::endl;
        }
        else if (state2type[res.first] != TokenType::EMPTY) {
            TokenType type = state2type[res.first];
            std::string content = input.substr(position, res.second - position - 1);
            if (type == TokenType::IDENTIFIER) {
                TokenType keywordType = keywords.lookup(content);
                if (keywordType != TokenType::EMPTY)
                    type = keywordType;
            }
            tokens.push_back(Token{type, content});
        }
        position = (position == (res.second - 1)) ? position + 1 : res.second - 1;
    }
    return tokens;
}

void init_lexer(void) {
    for (int i = 0 ; i < 128 ; i ++) char_mapping[i] = 0;
    for (int i = '0' ; i < '9' ; i ++) char_mapping[i] = 1;
    for (int i = 'a' ; i < 'z' ; i ++) char_mapping[i] = 2;
    for (int i = 'A' ; i < 'Z' ; i ++) char_mapping[i] = 2;
    char_mapping['_'] = 2;
    char_mapping[','] = 3;
    char_mapping['['] = 4;
    char_mapping[']'] = 5;
    char_mapping[':'] = 6;
    char_mapping['"'] = 7;
    char_mapping['\\'] = 8;
    char_mapping['.'] = 10;
    char_mapping['#'] = 11;
    char_mapping['\n'] = 12;

    keywords.insert("assign",TokenType::ASSIGN);
    keywords.insert("add",TokenType::ADD);
    keywords.insert("sub",TokenType::SUB);
    keywords.insert("mul",TokenType::MUL);
    keywords.insert("smul",TokenType::SMUL);
    keywords.insert("div",TokenType::DIV);
    keywords.insert("sdiv",TokenType::SDIV);
    keywords.insert("rem",TokenType::REM);
    keywords.insert("srem",TokenType::SREM);
    keywords.insert("and",TokenType::AND);
    keywords.insert("or",TokenType::OR);
    keywords.insert("xor",TokenType::XOR);
    keywords.insert("not",TokenType::NOT);
    keywords.insert("shl",TokenType::SHL);
    keywords.insert("ashr",TokenType::ASHR);
    keywords.insert("lshr",TokenType::LSHR);
    keywords.insert("neg",TokenType::NEG);
    keywords.insert("call",TokenType::CALL);
    keywords.insert("with",TokenType::WITH);
    keywords.insert("ret",TokenType::RET);
    keywords.insert("br",TokenType::BR);
    keywords.insert("goto",TokenType::GOTO);
    keywords.insert("cz",TokenType::CZ);
    keywords.insert("cnz",TokenType::CNZ);
    keywords.insert("ceq",TokenType::CEQ);
    keywords.insert("cne",TokenType::CNE);
    keywords.insert("cgt",TokenType::CGT);
    keywords.insert("cgts",TokenType::CGTS);
    keywords.insert("cge",TokenType::CGE);
    keywords.insert("cges",TokenType::CGES);
    keywords.insert("clt",TokenType::CLT);
    keywords.insert("clts",TokenType::CLTS);
    keywords.insert("cle",TokenType::CLE);
    keywords.insert("cles",TokenType::CLES);
    keywords.insert("variables",TokenType::VARIABLES);
    keywords.insert("byte",TokenType::TYPE_BYTE);
    keywords.insert("word",TokenType::TYPE_WORD);
    keywords.insert("dword",TokenType::TYPE_DWORD);
    keywords.insert("qword",TokenType::TYPE_QWORD);
    keywords.insert("float",TokenType::TYPE_FLOAT);
    keywords.insert("double",TokenType::TYPE_DOUBLE);
    keywords.insert("pointer",TokenType::TYPE_POINTER);
    keywords.insert("variables",TokenType::VARIABLES);
    keywords.insert("label",TokenType::LABEL);
    keywords.insert("define",TokenType::DEFINE);
    keywords.insert("nop",TokenType::NOP);
    keywords.insert("select",TokenType::SELECT);
}