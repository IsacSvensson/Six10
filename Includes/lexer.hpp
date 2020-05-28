#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>

enum Type : short{
    // Input, letters and numbers
    STRING, // done
    INTEGER, // done
    FLOAT, // done
    IDENTIFIER, // done
    TABB, // done
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
    RELATIONALOP,
    LOGICALOP,
    BITWISEOP,
    // WHITESPACE
    SPACE, // done
    WIN_EOL,
    EOL, // done
    WIN_EOF, // done
    EOF_,
    // INVALID
    INVALID
};
struct token;
void lexer(std::string sourceCode);

#endif