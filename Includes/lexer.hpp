#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>
#include "error.hpp"
#include "position.hpp"


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
    short type;
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

class Lexer{
    std::string sourceCode;
    Position pos;
    std::string filename;
    std::string::iterator it;
    std::vector<struct Token> tokens;
    void advance();
public:
    Lexer(std::string sc, std::string fn) : sourceCode(sc), pos(fn, sourceCode), filename(fn), it(sourceCode.begin()) {};
    std::pair<std::vector<struct Token>, Error*> makeTokens();
};


#endif