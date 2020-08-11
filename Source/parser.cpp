#include "interpreter.hpp"
#include "parser.hpp"

Token* Parser::advance(){
    tokIndex++;
    if (tokIndex < tokens.size())
        return &tokens[tokIndex];
    return nullptr;
}

ParseResult* Parser::call(){
    auto res = new ParseResult();
    Token* tok = &tokens[tokIndex];
    auto atom = res->registerResult(this->atom());
    tok = &tokens[tokIndex];
    if (res->error) return res;

    std::vector<astNode*> argNodes;
    if (tok->value == "("){
        res->registerAdvancement();
        advance();
        tok++;

        if(tok->value == ")"){
            res->registerAdvancement();
            advance();
            tok++;
        }
        else
        {
            argNodes.push_back(res->registerResult(expr()));
            if (res->error)
                return res->failure((Error*)(new InvalidSyntaxError(tokens[tokIndex].posEnd->filename, *tokens[tokIndex].posStart, *tokens[tokIndex].posEnd,
                "'+', '-', var, int, for, while, def, float or an identifier")));
            tok = &tokens[tokIndex];
            while(tok->value == ","){
                res->registerAdvancement();
                advance();
                tok++;

                argNodes.push_back(res->registerResult(expr()));
                if (res->error) return res;
                tok = &tokens[tokIndex];
            }
            if (tok->value != ")")
                return res->failure((Error*)(new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected ',' or ')'")));

            res->registerAdvancement();
            advance();
            tok++;
        }
    return res->success((astNode*)new CallNode(atom, argNodes));
    }
    return res->success(atom);
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
    else if (tok->value == "for"){
        auto forExpr = res->registerResult(this->forExpr());
        if (res->error)
            return res;
        return res->success(forExpr);
    }
    else if (tok->value == "while"){
        auto whileExpr = res->registerResult(this->whileExpr());
        if (res->error)
            return res;
        return res->success(whileExpr);
    }
    else if (tok->value == "def"){
        auto funcDef = res->registerResult(this->funcDef());
        if (res->error)
            return res;
        return res->success(funcDef);
    }
    return res->failure((Error*)(new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected '+', '-', int, float or an identifier")));
}

ParseResult* Parser::funcDef(){
    auto res = new ParseResult();
    Token* tok = &tokens[tokIndex];

    if (tok->value != "def"){
        return res->failure((Error*)new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected 'def'"));
    }

    res->registerAdvancement();
    advance();
    tok++;

    Token* varNameTok = nullptr;
    if (tok->type == IDENTIFIER){
        varNameTok = tok;
        res->registerAdvancement();
        advance();
        tok++;
        if (tok->value != "(")
            return res->failure((Error*)new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected '('"));
    }
    else
        if (tok->value != "(")
            return res->failure((Error*)new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected an identifier or '('"));

    res->registerAdvancement();
    advance();
    tok++;

    std::vector<Token> argNameToks;

    if (tok->type == IDENTIFIER){
        argNameToks.push_back(*tok);
        res->registerAdvancement();
        advance();
        tok++;

        while (tok->value == ",")
        {
            res->registerAdvancement();
            advance();
            tok++;

            if (tok->type != IDENTIFIER)
                return res->failure((Error*)new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected identifier"));
            
            argNameToks.push_back(*tok);
            res->registerAdvancement();
            advance();
            tok++;
        }
        if (tok->value != ")")
            return res->failure((Error*)new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected ',' or ')'"));
    }
    else
    {
        if (tok->value != ")")
            return res->failure((Error*)new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected identifier or ')'"));
    }
    res->registerAdvancement();
    advance();
    tok++;

    if (tok->value != "->"){
        return res->failure((Error*)new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected '->'"));
    }
    res->registerAdvancement();
    advance();
    tok++;

    auto nodeToReturn = res->registerResult(expr());
    if (res->error) return res;

    return res->success((astNode*)new FuncDefNode(varNameTok, argNameToks, nodeToReturn));
}

ParseResult* Parser::forExpr(){
    auto res = new ParseResult();
    Token* tok = &tokens[tokIndex];

    if (tok->value != "for"){
        return res->failure((Error*)new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected 'for'"));
    }

    res->registerAdvancement();
    advance();
    if (tokIndex < tokens.size()) tok++;

    if (tok->type != IDENTIFIER)
        return res->failure((Error*)new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected identifier"));
    
    auto varName = tok;
    res->registerAdvancement();
    advance();
    if (tokIndex < tokens.size()) tok++;

    if (tok->value != "=")
        return res->failure((Error*)new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected '='"));
    
    res->registerAdvancement();
    advance();
    if (tokIndex < tokens.size()) tok++;

    auto startVal = res->registerResult(expr());
    if (res->error) return res;

    res->registerAdvancement();
    advance();
    if (tokIndex < tokens.size()) tok++;


    if (tok->value != "to")
        return res->failure((Error*)new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected to"));

    auto endVal = res->registerResult(expr());
    if (res->error) return res;

    astNode* stepVal = nullptr;
    if (tokens[tokIndex].value == "step"){
        res->registerAdvancement();
        advance();
        if (tokIndex < tokens.size()) tok++;

        stepVal = res->registerResult(expr());
        if (res->error) return res;
    }
    tok = &tokens[tokIndex];
    if (tok->value != "then")
        return res->failure((Error*)new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected 'then'"));

    res->registerAdvancement();
    advance();
    if (tokIndex < tokens.size()) tok++;

    auto body = res->registerResult(expr());
    if (res->error) return res;

    return res->success((astNode*)new ForNode(*varName, startVal, endVal, stepVal, body));
}

ParseResult* Parser::whileExpr(){
    auto res = new ParseResult();
    Token* tok = &tokens[tokIndex];

    if (tok->value != "while")
        return res->failure((Error*)new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected 'while'"));
    
    res->registerAdvancement();
    advance();

    auto condition = res->registerResult(expr());
    if (res->error) return res;
    tok = &tokens[tokIndex];

    if (tok->value != "then")
        return res->failure((Error*)new InvalidSyntaxError(tok->posStart->filename, *tok->posStart, *tok->posEnd, "Expected 'then'"));
    
    res->registerAdvancement();
    advance();

    auto body = res->registerResult(expr());
    if (res->error) return res;

    return res->success((astNode*)new WhileNode(condition, body)); 
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
    auto left = res->registerResult(call());
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
        return res->success((astNode*)new UnOpNode(UNARYOP, node, &opToken));
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
    if (opToken->type == EOL || opToken->type == EOF_ || opToken->value == "elif" || opToken->value == "else" || opToken->value == "to" || opToken->value == "then" || opToken->value == "step" || opToken->value == "," || opToken->value == ")")
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