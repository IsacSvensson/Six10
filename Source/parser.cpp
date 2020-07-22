#include "interpreter.hpp"
#include "parser.hpp"

Token* Parser::advance(){
    tokIndex++;
    if (tokIndex < tokens.size())
        return &tokens[tokIndex];
    return nullptr;
}

ParseResult* Parser::atom(){
    auto res = new ParseResult();
    Token* tok = &tokens[tokIndex];

    if (tok->type == INTEGER || tok->type == FLOAT){
        res->registerAdvancement();
        advance();
        auto toReturn = new numberNode((Type)tok->type, tok);
        return res->success((astNode*)toReturn);
    }
    else if (tok->type == PARENTHESES && tok->value == "(")
    {
        res->registerAdvancement();
        advance();
        auto expression = res->registerResult(this->expr());
        if (res->error)
            return res;
        if (tokens[tokIndex].value == ")"){
            res->registerAdvancement();
            advance();
            return res->success(expression);}
        else
        {
            return res->failure((Error*)(new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected ')'")));
        }
    }
    else if (tok->type == IDENTIFIER){
        res->registerAdvancement();
        advance();
        auto toReturn = new VarAccessNode(tok);
        return res->success((astNode*)toReturn);
    }
    else if (tok->value == "if"){
        auto ifExpr = res->registerResult(this->ifExpr());
        if (res->error)
            return res;
        return res->success(ifExpr);
    }
    return res->failure((Error*)(new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected '+', '-', int, float or an identifier")));
}

ParseResult* Parser::ifExpr(){
    auto res = new ParseResult();
    std::vector<std::pair<astNode*, astNode*>> cases;
    astNode* elseCase = nullptr;
    Token* tok;

    if (tokens[tokIndex].value != "if"){
        tok = &tokens[tokIndex];
        return res->failure((Error*)new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected 'if'"));
    }

    res->registerAdvancement();
    advance();

    auto condition = res->registerResult(expr());
    if (res->error)
        return res;

    if (tokens[tokIndex].value != "then"){
        tok = &tokens[tokIndex];
        return res->failure((Error*)new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected 'then'"));
    }

    res->registerAdvancement();
    advance();

    auto expr = res->registerResult(this->expr());
    if (res->error)
        return res;
    cases.push_back(std::make_pair(condition, expr));

    while (tokens[tokIndex].value == "elif"){
        res->registerAdvancement();
        advance();

        auto condition = res->registerResult(this->expr());
        if (res->error)
            return res;

        if (tokens[tokIndex].value != "then"){
            tok = &tokens[tokIndex];
            return res->failure((Error*)new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected 'then'"));
        }

        res->registerAdvancement();
        advance();

        auto expr = res->registerResult(this->expr());
        if (res->error)
            return res;
        cases.push_back(std::make_pair(condition, expr));
    }

    if (tokens[tokIndex].value == "else"){
        res->registerAdvancement();
        advance();

        elseCase = res->registerResult(this->expr());
        if (res->error)
            return res;
    }
    return res->success((astNode*)new IfNode(cases, elseCase));

}

ParseResult* Parser::power(){
    auto res = new ParseResult();
    auto left = res->registerResult(atom());
    if (res->error)
        return res;
    Token* opToken;
    astNode* right;
    while (tokens[tokIndex].value == "^" ){
        opToken = &tokens[tokIndex];
        res->registerAdvancement();
        advance();
        right = res->registerResult(factor());
        if (res->error)
            return res;
        left = (astNode*)(new binOpNode(ARITHMETICOP, left, opToken, right));
    }
    return res->success(left);
}

ParseResult* Parser::factor(){
    auto res = new ParseResult();
    Token* tok = &tokens[tokIndex];
    
    if (tok->type == ARITHMETICOP && (tok->value == "+" || tok->value == "-")){
        res->registerAdvancement();
        advance();
        auto factor = res->registerResult(this->factor());
        if (res->error)
            return res;
        return res->success((astNode*)(new UnOpNode(UNARYOP, factor, tok)));
    }
    
    return power();
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
        res->registerAdvancement();
        advance();
        right = res->registerResult(factor());
        if (res->error)
            return res;
        left = (astNode*)(new binOpNode(ARITHMETICOP, left, opToken, right));
    }
    return res->success(left);
}

ParseResult* Parser::arithmExpr(){
    auto res = new ParseResult();
    auto left = res->registerResult(term());
    if (res->error)
        return res;
    Token* opToken;
    astNode* right;

    while (tokens[tokIndex].value == "+" || tokens[tokIndex].value == "-" ){
        opToken = &tokens[tokIndex];
        res->registerAdvancement();
        advance();
        right = res->registerResult(term());
        if (res->error)
            return res->failure((Error*)(new InvalidSyntaxError(tokens[tokIndex].posEnd->filename, *tokens[tokIndex].posStart, *tokens[tokIndex].posEnd,
            "'+', '-', var, int, float or an identifier")));
        left = (astNode*)(new binOpNode(ARITHMETICOP, left, opToken, right));
    }
    return res->success(left);
}

ParseResult* Parser::compExpr(){
    auto res = new ParseResult();
    if (tokens[tokIndex].value == "not" || tokens[tokIndex].value == "!"){
        auto opToken = tokens[tokIndex];
        res->registerAdvancement();
        advance();
        auto node = res->registerResult(compExpr());
        if (res->error)
            return res;
        return res->success((astNode*)new UnOpNode(UNARYOP, node, &tokens[tokIndex]));
        }
    auto node = res->registerResult(arithmExpr());
    if (res->error)
        return res->failure((Error*)(new InvalidSyntaxError(tokens[tokIndex].posStart->filename, *tokens[tokIndex].posStart, *tokens[tokIndex].posEnd, "Expected int, float, identifier, '+', '-', '(' or 'not'")));
    return res->success(node);
}

ParseResult* Parser::expr(){
    auto res = new ParseResult();
    Token* varName;

    if (tokens[tokIndex].value == "var"){
        res->registerAdvancement();
        advance();
        if (tokens[tokIndex].type != IDENTIFIER)
            return res->failure((Error*)(new InvalidSyntaxError(tokens[tokIndex].posStart->filename, 
            *tokens[tokIndex].posStart, *tokens[tokIndex].posEnd, "Expected identifier")));
        varName = &tokens[tokIndex];
        res->registerAdvancement();
        advance();
        if (tokens[tokIndex].type != ASSIGNMENTOP)
            return res->failure((Error*)(new InvalidSyntaxError(tokens[tokIndex].posStart->filename, 
            *tokens[tokIndex].posStart, *tokens[tokIndex].posEnd, "Expected assignment operator")));
        res->registerAdvancement();
        advance();
        auto expression = res->registerResult(expr());
        if (res->error)
            return res;
        return res->success((astNode*)(new VarAssignNode(varName, expression)));
        }
    auto left = res->registerResult(compExpr());
    if (res->error)
        return res;
    Token* opToken;
    astNode* right;
    
    opToken = &tokens[tokIndex];
    if (opToken->type == EOL || opToken->type == EOF_ || opToken->value == "elif" || opToken->value == "else")
        return res->success(left);
    res->registerAdvancement();
    advance();
    right = res->registerResult(compExpr());
    if (res->error)
        return res->failure((Error*)(new InvalidSyntaxError(tokens[tokIndex].posEnd->filename, *tokens[tokIndex].posStart, *tokens[tokIndex].posEnd,
        "'+', '-', var, int, float or an identifier")));
    left = (astNode*)(new binOpNode(ARITHMETICOP, left, opToken, right));
    return res->success(left);
}

ParseResult* Parser::parse(){
    auto res = expr();
    if (res->error){
        return res;
    }
    else if(!res->error && tokens[tokIndex].type != EOF_)
        return res->failure((Error*)(new InvalidSyntaxError(tokens[tokIndex].posStart->filename, 
            *tokens[tokIndex].posStart, *tokens[tokIndex].posEnd, "Expected an '+', '-', '*' or '/'")));

    return res;
}

astNode* ParseResult::registerResult(ParseResult* res){
    advanceCount++;
    if(res->error)
        error = res->error;
    return res->node;
}

void ParseResult::registerAdvancement(){
    advanceCount++;
}

ParseResult* ParseResult::success(astNode* node){
    this->node = node;
    return this;
}
ParseResult* ParseResult::failure(Error* error){
    if (!(this->error) || advanceCount == 0)
        this->error = error;
    return this;
}