#include <iostream>
#include <queue>
#include "typeCheckers.hpp"
#include "lexer.hpp"
#include "nodes.hpp"
#include "parser.hpp"
#include <fstream>

void getSourceCode(std::string path, std::string &sourceCode){
    std::ifstream ifs(path);
    if (ifs)
        sourceCode.assign((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    else{
        std::cout << "Error: Could not open path: " << path;
        abort();
    }
}

int main(int argc, char* argv[]){
    if (argc > 1){
        std::string opt = argv[1];
        if (opt == "-shell")
            while (true)
            {
                std::string text;
                printf(">> ");
                std::getline(std::cin, text);
                if (text == "q")
                    return 0;
                Parser p(lexer(text));
                p.run();
                std::cout << std::endl;
            }
        else if(opt == "-h" || opt == "-hjälp")
        {
            std::cout << "\nOptions:\n----------------------------------------------------------------\n" 
                << " -h, -help\t\tHelp, show this display\n"
                << " -shell\t\t\tOpens CLI\n" << " filename\t\tReads local file to lexer and parser\n" 
                << "----------------------------------------------------------------\n"
                << "   Github repo: https://github.com/IsacSvensson/Six10\n";
        }
        else
        {
            std::string sourceCode;
            getSourceCode(opt, sourceCode);
            auto q = lexer(sourceCode);
            Parser parser(q);
            parser.run();
        }
    }
    else
    {
        printf("Error: Not enough arguments - \"-h\" for help");
    }
}