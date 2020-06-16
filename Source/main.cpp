#include <iostream>
#include <queue>
#include "typeCheckers.hpp"
#include "lexer.hpp"
#include "nodes.hpp"
#include "parser.hpp"
#include <fstream>

void getSourceCode(std::string path, std::string &sourceCode){
    std::ifstream ifs(path);
    sourceCode.assign((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
}

int main(){
    std::string sourceCode;
    getSourceCode("./Documentation/binoptest.six10", sourceCode);
    auto q = lexer(sourceCode);
    Parser parser(q);
    parser.run();
}