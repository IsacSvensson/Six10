#include "parser.hpp"
#include "lexer.hpp"

Token* Parser::advance(){
    tokIndex++;
    if (tokIndex < tokens.size())
        return &tokens[tokIndex];
    return nullptr;
}


ParseResult* Parser::factor(){
    auto res = new ParseResult();
    Token* tok = &tokens[tokIndex];
    
    if (tok->type == INTEGER || tok->type == FLOAT){
        res->registerResult(nullptr, advance());
        auto toReturn = new numberNode((Type)tok->type, tok);
        return res->success((astNode*)toReturn);
    }
    return res->failure((Error*)(new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected int or float")));
}

ParseResult* Parser::term(){
    auto res = new ParseResult();
    auto left = res->registerResult(factor());
    if (res->error)
        return res;
    Token* opToken;
    astNode* right;
    while (tokens[tokIndex].value == "*" || tokens[tokIndex].value == "/" ){
        opToken = &tokens[tokIndex];
        res->registerResult(nullptr, advance());
        right = res->registerResult(factor());
        if (res->error)
            return res;
        left = (astNode*)(new binOpNode(ARITHMETICOP, left, opToken, right));
    }
    return res->success(left);
}

ParseResult* Parser::expr(){
    auto res = new ParseResult();
    auto left = res->registerResult(term());
    if (res->error)
        return res;
    Token* opToken;
    astNode* right;

    while (tokens[tokIndex].value == "+" || tokens[tokIndex].value == "-" ){
        opToken = &tokens[tokIndex];
        res->registerResult(nullptr, advance());
        right = res->registerResult(term());
        if (res->error)
            return res;
        left = (astNode*)(new binOpNode(ARITHMETICOP, left, opToken, right));
    }
    return res->success(left);
}

ParseResult* Parser::parse(){
    auto res = expr();
    if (res->error){
        std::cout << res->error->toString();
        return nullptr;
    }
    else if(!res->error && tokens[tokIndex].type != EOF_)
        return res->failure((Error*)(new InvalidSyntaxError(tokens[tokIndex].posStart->filename, 
            *tokens[tokIndex].posStart, *tokens[tokIndex].posEnd, "Expected an '+', '-', '*' or '/'")));
    std::cout << res->node << std::endl; 
    printTree(res->node);

    return res;
}

void Parser::printTree(astNode* tree){
    switch (tree->nodeType)
    {
    case ARITHMETICOP:
        std::cout << "(";
        printTree(((binOpNode*)tree)->left);
        std::cout << (((binOpNode*)tree)->op->value);
        printTree(((binOpNode*)tree)->right);
        std::cout << ")";
        break;
    case INTEGER:
        std::cout << ((numberNode*)tree)->tok.value;
        break;
    case FLOAT:
        std::cout << ((numberNode*)tree)->tok.value;
        break;
    default:
        break;
    } 
}

astNode* ParseResult::registerResult(ParseResult* pr, Token* n){
    if (n)
        return nullptr;
    if(pr->error)
        error = pr->error;
    return pr->node;
}

ParseResult* ParseResult::success(astNode* node){
    this->node = node;
    return this;
}
ParseResult* ParseResult::failure(Error* error){
    this->error = error;
    return this;
}