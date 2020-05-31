#include <iostream>
#include <queue>
#include "typeCheckers.hpp"
#include "lexer.hpp"

int main(){
    auto q = lexer("foreach number in range(0, 10):\n\toutput(\"Printing number: + number\")/# here is a multiline comment\\n this is the second line #/ # here is a regular comment\n");
    while (!q.empty()){
        std::cout << '<' << getType(q.front()) << '>';
        int spaces = 20 - (getType(q.front()).size()+2);
        for (int i = -1; i < spaces; i++)
            std::cout << ' ';
        std::cout << ": \"" << q.front().value << "\"" << std::endl;
        q.pop();
    }
}