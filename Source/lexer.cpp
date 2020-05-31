#include <utility>
#include <queue>
#include "typeCheckers.hpp"
#include "lexer.hpp"

std::queue<Token> lexer(std::string sourceCode){
    auto iterator = sourceCode.begin();
    std::queue<Token> q;
    while (iterator < sourceCode.end())
    {
        auto token = isType(iterator, sourceCode.end());
        q.push(Token(token, iterator));
    }
    q.push(Token(EOF_, ""));
    return q;
}