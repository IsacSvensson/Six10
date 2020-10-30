#include "six10.hpp"

Six10::Six10(SymbolTable* symtab) : symtab(symtab), parser(), lexer(), interpreter() {}

std::pair<Value*, Error*> Six10::run(std::string code, std::string fn){
    lexer.reset(code, fn);
    auto tokens = lexer.makeTokens();

    if(tokens.second){
        return std::make_pair(nullptr, tokens.second);
    }

    parser.reset(tokens.first);
    auto res = parser.parse();
    if (res->error){
            return std::make_pair(nullptr, res->error);
        }

    interpreter.setNode(res->node);
    Context* context = new Context("<program>");
    context->symTab = symtab;
    auto result = interpreter.visit(res->node, context);
    
    return std::make_pair(result->value, result->error);
}