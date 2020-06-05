#include "typeCheckers.hpp"

class numberNode{
    Token tok;
public:
    numberNode(Token* token) : tok(*token) {};
};

template <typename tokA, typename tokB>
class binOpNode{
    tokA* leftNode;
    tokB* rightNode;
    Token op;
};