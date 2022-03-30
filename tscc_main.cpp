#include <iostream>
#include "tscc_lexer.h"

int main(int argc, const char **argv) {
    init_lexer();
    std::string input = "";
    std::string str = "";
    do {
        input += str;
        std::getline(std::cin, str);
    }while (str != "__endinput__");
    auto tokens = lex(input);
    for (auto tok : tokens) {
        std::cout << "(" << to_string(tok.type) << ":" << tok.content << ");";
    }
    std::cout << std::endl;
    return 0;
}