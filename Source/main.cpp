#include <iostream>
#include <queue>
#include "interpreter.hpp"
#include "nodes.hpp"
#include "parser.hpp"
#include "test.hpp"
#include "helpers.hpp"
#include "six10.hpp"

int main(int argc, char* argv[]){
    if (argc > 1){
        std::string opt = argv[1];
        if (opt == "-shell"){
            Six10 obj;
            obj = Six10();
            std::string text;
            std::string cli = "Command line interface";
            while (true)
            {
                printf(">> ");
                std::getline(std::cin, text);
                if (text == "q")
                    return 0;
                auto res = obj.run(text, cli);
                if (res.second)
                    if (res.second->type == RTERROR)
                        std::cout << ((RuntimeError*)res.second)->toString();
                    else
                        std::cout << res.second->toString() << std::endl;
            } }
        else if (opt == "-test"){ 
            auto symtab = new SymbolTable();
            testAllFunc(symtab);
            delete symtab;
        }
        else if(opt == "-h" || opt == "-help"){
            std::cout << "\nSix10 - Programming Language\n\nOptions:\n----------------------------------------------------------------\n" 
                << " -h, -help\t\tHelp, show this display\n"
                << " -shell\t\t\tOpens CLI\n" << " filename.six10\t\tRuns local file\n"
                << " -test\t\t\tRuns all unit tests\n"
                << "----------------------------------------------------------------\n"
                << "   Github repo: https://github.com/IsacSvensson/Six10\n";
        }
        else
        {
            Six10 obj;
            obj = Six10();
            std::string sourceCode;
            getSourceCode(opt, sourceCode);
            obj.run(sourceCode, opt);
        }
    }
    else
    {
        printf("Error: Not enough arguments - \"-h\" for help");
    }
}