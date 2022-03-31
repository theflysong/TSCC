#include <iostream>
#include "tscc_lexer.h"

int main(int argc, const char **argv) {
    init_lexer();
    std::string input = "";
    std::string str = "";
    do {
        input += (str + "\n");
        std::getline(std::cin, str);
    }while (str != "__endinput__");
    auto raw_tokens = lex(input);
    for (auto tok : raw_tokens) {
        if (tok.type == TokenType::NEWLINE)
            std::cout << "(" << to_string(tok.type) << ":" << "\\n" << ");" << std::endl;
        else
            std::cout << "(" << to_string(tok.type) << ":" << tok.content << ");";
    }
    std::cout << std::endl;
    decltype(raw_tokens) tokens;
    for (auto tok : raw_tokens) {
        if (tok.type != TokenType::NEWLINE && tok.type != TokenType::COMMENT)
            tokens.push_back(tok);
    }
    return 0;
}