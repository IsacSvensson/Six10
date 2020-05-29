#include <iostream>
#include <utility>
#include <queue>
#include "typeCheckers.hpp"
#include "lexer.hpp"

void lexer(std::string sourceCode){
    auto iterator = sourceCode.begin();
    std::queue<Token> q;
    while (iterator < sourceCode.end())
    {
        auto token = isType(iterator);
        q.push(Token(token, iterator));
        std::cout << '<' << getType(q.back()) << '>';
        int tabs = (24 - (getType(q.back()).size()+10))/8;
        for (int i = -1; i < tabs; i++)
            std::cout << '\t';
        std::cout << ": " << q.back().value << std::endl;
    }
}

int main(){
    lexer("foreach number in range(0, 10):\n    output(\"Printing number: \" + number)");
}