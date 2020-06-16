#include <utility>
#include "typeCheckers.hpp"
#include "lexer.hpp"

std::vector<Token> lexer(std::string sourceCode){
    auto iterator = sourceCode.begin();
    std::vector<Token> v;
    while (iterator < sourceCode.end())
    {
        auto token = isType(iterator, sourceCode.end());
        if (token.first == SPACE){
            iterator++;
            continue;
            }
        v.push_back(Token(token, iterator));
    }
    v.push_back(Token(EOF_, ""));
    return v;
}