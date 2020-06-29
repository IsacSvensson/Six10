#include <iostream>
#include <queue>
#include "interpreter.hpp"
#include "typeCheckers.hpp"
#include "lexer.hpp"
#include "nodes.hpp"
#include "parser.hpp"
#include "error.hpp"
#include "context.hpp"
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

std::pair<Number*, Error*> run(std::string code, std::string fn){
    Lexer lex(code, fn);
    auto tokens = lex.makeTokens();

    if(tokens.second){
        return std::make_pair(nullptr, tokens.second);
    }

    Parser p(tokens.first);
    auto res = p.parse();
    if (res->error){
            return std::make_pair(nullptr, res->error);
        }

    Interpreter interpreter(res->node);
    Context* context = new Context("<program>");
    auto result = interpreter.visit(res->node, context);
    
    return std::make_pair(result->value, result->error);
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
                std::string cli = "Command line interface";
                auto res = run(text, cli);
                if (res.second)
                    if (res.second->type == RTERROR)
                        std::cout << ((RuntimeError*)res.second)->toString();
                    else
                        std::cout << res.second->toString() << std::endl;
                else
                    std::cout << "= " << res.first->value << std::endl;
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
            run(sourceCode, opt);
        }
    }
    else
    {
        printf("Error: Not enough arguments - \"-h\" for help");
    }
}