#include <iostream>
#include <utility>
#include "typeCheckers.hpp"
#include "lexer.hpp"

struct Token
{
    short type;
    std::string value;
    Token(Type t, std::string s) : type(t), value(s) {};
};


void lexer(std::string sourceCode){
    auto iterator = sourceCode.begin();

    while (iterator < sourceCode.end())
    {
        auto token = isType(iterator);
        std::cout << token.first << ' ' << token.second << std::endl;
        iterator += token.second;
    }
}

int main(){
    lexer("/#153.18\r\n<and ,        >#/int");
}