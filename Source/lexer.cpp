#include <iostream>
#include <utility>
#include <queue>
#include "typeCheckers.hpp"
#include "lexer.hpp"

struct Token
{
    short type;
    std::string value;
    Token(Type t, std::string s) : type(t), value(s) {};
    Token(std::pair<Type, int> tokenPair, std::string::iterator &i){
        type = tokenPair.first;
        value = "";
        for (int c = 0; c < tokenPair.second; c++)
            value += *i++;
        if (value == "\n")
            value = "\\n";
        else if (value == "    ")
            value = "\t";
    };
        
};

void lexer(std::string sourceCode){
    auto iterator = sourceCode.begin();
    std::queue<Token> q;
    while (iterator < sourceCode.end())
    {
        auto token = isType(iterator);
        q.push(Token(token, iterator));
        std::cout << q.back().type << ' ' << q.back().value << std::endl;
    }
}

int main(){
    lexer("foreach number in range(0, 10):\n    output(\"Printing number: \" + number)");
}