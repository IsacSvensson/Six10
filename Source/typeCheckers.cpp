#include "lexer.hpp"

std::string keywords[9]{"and", "or", "not", "foreach", "while", "in", "if", "else if", "else"};
std::string datatypes[5]{"int", "float", "bool", "char", "string"};
std::string assignmentOperators[6]{"=", "+=", "-=", "*=", "/=", "%="};
std::string arithmeticOperators[8]{"+", "-", "/", "*", "%", "**", "++", "--"};
char parentheses[8]{'(', ')', '{', '}', '[', ']', '<', '>'};

bool isParentheses(std::string::iterator it){
    bool result = false;
    for (char &c : parentheses)
        if (*it == c)
        {
            result = true;
            break;
        }
    return result;
}

bool isAssignmentOperator(std::string str){
    bool result = false;
    for (std::string &op : assignmentOperators)
        if (str == op)
        {
            result = true;
            break;
        }
    return result;
}

bool isArithmeticOperator(std::string str){
    bool result = false;
    for (std::string &op : arithmeticOperators)
        if (str == op)
        {
            result = true;
            break;
        }
    return result;
}

bool isKeyword(std::string str){
    bool result = false;
    for (std::string &keyword : keywords)
        if (str == keyword)
        {
            result = true;
            break;
        }
    return result;
}

bool isDataType(std::string str){
    bool result = false;
    for (std::string &datatype : datatypes)
        if (str == datatype)
        {
            result = true;
            break;
        }
    return result;
}

std::pair<Type, int> isType(std::string::iterator it){
    int len = 0;
    auto newIt = it;
    bool notFound = true;
    
    if (isalpha(*it) || *it == '_'){
        std::string string = "";
        while ((isalpha(*newIt) || isdigit(*newIt)) || (*newIt == '_'))
        {
            string += *newIt;
            newIt++;
            len++;
        }
        if (isKeyword(string))
            return std::make_pair(KEYWORD, len);
        if (isDataType(string))
            return std::make_pair(DATATYPE, len);
        return std::make_pair(IDENTIFIER, len);
    }
    else if (*it == '"'){
        while (notFound)
        {
            if ((*newIt == '"') && len > 1)
                notFound = false;
            newIt++;
            len++;
        }
        return std::make_pair(STRING, len);
        }
    else if (isdigit(*it))
    {
        short dot = 0;
        while (isdigit(*newIt) || *newIt == '.')
        {
            if (*newIt == '.')
                dot++;
            newIt++;
            len++;
        }
        if (dot == 0)
            return std::make_pair(INTEGER, len);
        else if (dot == 1)
            return std::make_pair(FLOAT, len);
        else
            return std::make_pair(INVALID, len);
    }
    else if (*it == ' '){
        while (*newIt == ' ' && len != 4)
        {
            len++;
            newIt++;
        }
        if (len == 4)
            return std::make_pair(TABB, 4);
        else
            return std::make_pair(SPACE, 1);
    }
    else if (*it == '\n')
        return std::make_pair(EOL, 1);
    else if (*it == '\r' && *(it+1) == '\n')
        return std::make_pair(WIN_EOL, 2);
    else if (isParentheses(it))
        return std::make_pair(PARENTHESES, 1);
    else if ((*it == '/' && *(it+1) == '#') || (*it == '#' && *(it+1) == '/'))
        return std::make_pair(MULTILINECOMMENT, 2);
    else if (*it == '#')
        return std::make_pair(COMMENT, 1);
    else if (*it == ',')
        return std::make_pair(COMMA, 1);
    else if (*it == '.')
        return std::make_pair(MEMBEROP, 1);
    
    return std::make_pair(INVALID, 1);
}