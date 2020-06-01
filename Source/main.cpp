#include <iostream>
#include <queue>
#include "typeCheckers.hpp"
#include "lexer.hpp"
#include <fstream>

void getSourceCode(std::string path, std::string &sourceCode){
    std::ifstream ifs(path);
    sourceCode.assign( (std::istreambuf_iterator<char>(ifs)),
                       (std::istreambuf_iterator<char>()));
}

int main(){
    std::string sourceCode;
    getSourceCode("./Documentation/kodexempel.six10", sourceCode);
    auto q = lexer(sourceCode);
    while (!q.empty()){
        std::cout << '<' << getType(q.front()) << '>';
        int spaces = 20 - (getType(q.front()).size()+2);
        for (int i = -1; i < spaces; i++)
            std::cout << ' ';
        std::cout << ": \"" << q.front().value << "\"" << std::endl;
        q.pop();
    }
}