#include <utility>
#include "typeCheckers.hpp"
#include "lexer.hpp"
#include "error.hpp"
#include "position.hpp"
#include <string>


std::pair<std::vector<Token>, Error*> Lexer::makeTokens(){
    while (it < sourceCode.end())
    {
        auto token = isType(it, sourceCode.end());
        if (token.first == SPACE){
            advance();
            continue;
            }
        else if (token.first == INVALID){
            Position posStart = pos;
            auto ch = std::string(1, *it);
            advance();
            return std::make_pair(tokens, (Error*) new IllegalCharError(ch, posStart, Position(pos)));
            }
        tokens.push_back(Token(token, it));
        for(int i = 0; i < token.second; i++)
            advance();
    }
    tokens.push_back(Token(EOF_, ""));
    return std::make_pair(tokens, nullptr);
}

void Lexer::advance(){
    char toSend = ' ';
    if (!tokens.empty())
        if (tokens.back().type == EOL)
            toSend = '\n';
    pos.advance(toSend);
    it++;
}