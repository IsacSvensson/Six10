#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>
#include <queue>

enum Type : short{
    // Input, letters and numbers
    STRING, // done
    INTEGER, // done
    FLOAT, // done
    IDENTIFIER, // done
    TAB, // done
    PARENTHESES, // done
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
    Token(Type t, std::string s) : type(t), value(s) {};
    Token(std::pair<Type, int> tokenPair, std::string::iterator &i){
        type = tokenPair.first;
        value = "";
        for (int c = 0; c < tokenPair.second; c++)
            value += *i++;
        if (value == "\n")
            value = "\\n";
        else if (value == "    ")
            value = "\t";
    };
};

std::queue<Token> lexer(std::string sourceCode);

#endif