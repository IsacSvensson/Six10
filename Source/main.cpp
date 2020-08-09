#include <iostream>
#include <queue>
#include "interpreter.hpp"
#include "nodes.hpp"
#include "parser.hpp"
#include "test.hpp"
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

static SymbolTable globalSymTab(100);

std::pair<Value*, Error*> run(std::string code, std::string fn){
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
    context->symTab = &globalSymTab;
    auto result = interpreter.visit(res->node, context);
    
    return std::make_pair(result->value, result->error);
}

int main(int argc, char* argv[]){
    globalSymTab.set("null", new Number(0, INTEGER));
    globalSymTab.set("True", new Number(1, INTEGER));
    globalSymTab.set("False", new Number(0, INTEGER));
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
                else if (res.first)
                    if (res.first->type == INTEGER || res.first->type == FLOAT)
                        std::cout << "= " << ((Number*)res.first)->value << std::endl;
        } else if (opt == "-test")
            testAllFunc();
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