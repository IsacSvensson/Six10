#pragma once

#include <string>
#include <vector>

class Position{
public:
    int index;
    int line;
    int col;
    std::string filename;
    std::string filetext;
    Position(std::string fn, std::string ftxt) : index(0), line(0), col(0), filename(fn), filetext(ftxt) {};
    Position(Position &rhs) : index(rhs.index), line(rhs.line), col(rhs.col), filename(rhs.filename), filetext(rhs.filetext) {};
    ~Position() {};
    void advance(char currentChar = ' ');
    std::string getPos();
};


enum Type : short{
    // Input, letters and numbers
    STRING, // done
    INTEGER, // done
    FLOAT, // done
    IDENTIFIER, // done
    TAB, // done
    PARENTHESES, // done
    COLON,
    KEYWORD, // done
    COMMENT, // Done
    MULTILINECOMMENT, //Done
    COMMA, // Done
    DATATYPE, // done
    // Operators
    ASSIGNMENTOP, // Checker function made
    MEMBEROP, // Done
    ARITHMETICOP, // Checker function made
    RELATIONALOP, // Checker function made
    LOGICALOP, // Checker function made
    BITWISEOP, // Not implemented for now
    UNARYOP,
    // Misc Nodes
    VARACCESSNODE,
    IFSTATMENT,
    FORLOOP,
    WHILELOOP,
    FUNCDEF,
    FUNCCALL,
    // WHITESPACE
    SPACE, // done
    WIN_EOL, // done
    EOL, // done
    EOF_,
    // INVALID
    INVALID
};

struct Token
{
    Type type;
    std::string value;
    Position* posStart;
    Position* posEnd;
    Token(Type t, std::string s, Position* ps = nullptr, Position* pe = nullptr) : type(t), value(s){ 
        if(ps)
            posStart = new Position(*ps); 
        if(pe) 
            posEnd = new Position(*pe); 
        else if (ps && !pe) {
            posEnd = new Position(*ps); 
            posEnd->advance(' ');}
    };
    Token(std::pair<Type, int> tokenPair, std::string::iterator &i, Position* s = nullptr, Position* e = nullptr){
        type = tokenPair.first;
        value = "";
        if (s){
            posStart = new Position(*s);
            if (e)
                posEnd = new Position(*e);
            else{
                posEnd = new Position(*s);
                for (int i = 0; i < tokenPair.second; i++)
                    posEnd->advance(' ');
            }
        }

        
        for (int c = 0; c < tokenPair.second; c++)
            value += *i++;
        if (value == "\n")
            value = "\\n";
        else if (value == "    ")
            value = "\t";
        i -= tokenPair.second;
    };
    ~Token(){
    }
};

class astNode{
public:
    Type nodeType;
    astNode* left;
    astNode* right;
    Position* posStart;
    Position* posEnd;
    astNode(Type nodeType, astNode* l = nullptr, astNode* r = nullptr) : 
        nodeType(nodeType), left(l), right(r), posStart(nullptr), posEnd(nullptr) {};
    friend std::ostream& operator<<(std::ostream& os , const astNode* node);
};

class numberNode{
public:
    Type nodeType;
    astNode* left;
    astNode* right;
    Position* posStart;
    Position* posEnd;
    Token tok;
    numberNode(Type nodeType, Token* token) : 
        nodeType(nodeType), left(nullptr), right(nullptr), posStart(token->posStart), posEnd(token->posEnd), tok(*token) {};
    numberNode(numberNode* node) : 
        nodeType(node->nodeType), left(node->left), right(node->right), posStart(node->posStart), posEnd(node->posEnd), tok(node->tok) {};
    Token* getToken() { return &tok; };
};

class binOpNode{
public:
    Type nodeType;
    astNode* left;
    astNode* right;
    Position* posStart;
    Position* posEnd;
    Token* op;
    binOpNode(Type nodeType, astNode* l, Token* op, astNode* r) : nodeType(nodeType), left(l), right(r), posStart(l->posStart), posEnd(r->posEnd), op(new Token(*op)) {};
    ~binOpNode() {delete left; delete right; delete op;};
    void doThis(){
    };
    void set(Type nodeType, astNode* l, Token* op, astNode* r);
};

class UnOpNode{
public:
    Type nodeType;
    astNode* left;
    astNode* right;
    Position* posStart;
    Position* posEnd;
    Token* op;
    UnOpNode(Type nodeType, astNode* node, Token* op) : nodeType(nodeType), left(node), posStart(op->posStart), posEnd(op->posEnd), op(new Token(*op)) {};
};

class VarAccessNode{
public:
    Type nodeType;
    astNode* left;
    astNode* right;
    Position* posStart;
    Position* posEnd;
    Token* varNameTok;
    VarAccessNode(Token* varNameTok) : nodeType(VARACCESSNODE), posStart(varNameTok->posStart), posEnd(varNameTok->posEnd), varNameTok(new Token(*varNameTok)) {};
};

class VarAssignNode{
public:
    Type nodeType;
    astNode* left;
    astNode* right;
    Position* posStart;
    Position* posEnd;
    Token* varNameTok;
    VarAssignNode(Token* varNameTok, astNode* nodeVal) : nodeType(ASSIGNMENTOP), left(nodeVal), posStart(varNameTok->posStart), posEnd(varNameTok->posEnd), varNameTok(new Token(*varNameTok)) {};
};

class IfNode : public astNode{
public:
    std::vector<std::pair<astNode*, astNode*>> cases;
    astNode* elseCase;
    IfNode(std::vector<std::pair<astNode*, astNode*>> c, astNode* eC) : cases(c), elseCase(eC), astNode(IFSTATMENT) {this->posStart = cases[0].first->posStart;
                                                                                                                        if (elseCase)
                                                                                                                            this->posEnd = elseCase->posEnd;
                                                                                                                        else
                                                                                                                            this->posEnd = cases[cases.size()-1].first->posEnd;
                                                                                                                        };
};

class ForNode : public astNode{
public:
    Token varNameTok;
    astNode* startValueNode;
    astNode* endValueNode;
    astNode* stepValueNode;
    astNode* bodyNode;
    ForNode(Token tok, astNode* start, astNode* end, astNode* step, astNode* body) : varNameTok(tok), startValueNode(start), endValueNode(end), stepValueNode(step), bodyNode(body), astNode(FORLOOP) {
        posStart = varNameTok.posStart; posEnd = bodyNode->posEnd;
    };
};

class WhileNode : public astNode{
public:
    astNode* conditionNode;
    astNode* bodyNode;
    WhileNode(astNode* condition, astNode* body) : conditionNode(condition), bodyNode(body), astNode(WHILELOOP) {posStart = condition->posStart; posEnd = bodyNode->posEnd;};
};

class FuncDefNode : public astNode{
public:
    Token* varNameTok;
    std::vector<Token> argNameToks;
    astNode* bodyNode;
    FuncDefNode(Token *vnt, std::vector<Token> ant, astNode* bn) : varNameTok(new Token(*vnt)), argNameToks(ant), bodyNode(bn), astNode(FUNCDEF) {
        if (varNameTok) posStart = varNameTok->posStart;
        else if (argNameToks.size()) posStart = argNameToks[0].posStart;
        else posStart = bodyNode->posStart;
        posEnd = bodyNode->posEnd;
    };
};

class CallNode : public astNode{
public:
    astNode* nodeToCall;
    std::vector<astNode*> argNodes;
    CallNode(astNode* ntc, std::vector<astNode*> an) : nodeToCall(ntc), argNodes(an), astNode(FUNCCALL) {
        posStart = nodeToCall->posStart;
        if (argNodes.size()) posEnd = argNodes[argNodes.size()-1]->posEnd;
        else posEnd = nodeToCall->posEnd;
    };
};