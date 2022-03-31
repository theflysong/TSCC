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
    auto tokens = lex(input);
    for (auto tok : tokens) {
        if (tok.type == TokenType::NEWLINE)
            std::cout << "(" << to_string(tok.type) << ":" << "\\n" << ");" << std::endl;
        else
            std::cout << "(" << to_string(tok.type) << ":" << tok.content << ");";
    }
    std::cout << std::endl;
    return 0;
}