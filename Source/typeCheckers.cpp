#include "lexer.hpp"

std::string keywords[11]{"and", "or", "not", "foreach", "while", "in", "if", "else if", "else", "is", "equal"};
std::string datatypes[5]{"int", "float", "bool", "char", "str"};
std::string assignmentOperators[6]{"=", "+=", "-=", "*=", "/=", "%="};
std::string arithmeticOperators[9]{"+", "-", "/", "*", "^", "%", "**", "++", "--"};
std::string relationalOperators[6]{"<", "<=", ">", ">=", "==", "!="};
std::string logicalOperators[6]{"&&", "||", "!"};
char parentheses[6]{'(', ')', '{', '}', '[', ']'};

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

bool islogicalOperator(std::string str){
    bool result = false;
    for (std::string &op : logicalOperators)
        if (str == op)
        {
            result = true;
            break;
        }
    return result;
}

bool isRelationalOperator(std::string str){
    bool result = false;
    for (std::string &op : relationalOperators)
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

std::pair<Type, int> isType(std::string::iterator it, std::string::iterator end){
    int len = 0;
    auto newIt = it;
    bool notFound = true;
    
    std::string twoChars;
    twoChars += *it;
    std::string singleChar = twoChars;
    twoChars += *(it+1);

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
        while (notFound && newIt < end)
        {
            if ((*newIt == '"') && len > 1)
                notFound = false;
            newIt++;
            len++;
        }
        if (notFound){
            return std::make_pair(INVALID, len);
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
            return std::make_pair(TAB, 4);
        else
            return std::make_pair(SPACE, 1);
    }
    else if (*it == '\t')
        return std::make_pair(TAB, 1);
    else if (*it == '\n')
        return std::make_pair(EOL, 1);
    else if (*it == '\r' && *(it+1) == '\n')
        return std::make_pair(WIN_EOL, 2);
    else if (isParentheses(it))
        return std::make_pair(PARENTHESES, 1);
    else if (*it == '/' && *(it+1) == '#'){
        while (notFound && newIt < end)
        {
            if (*newIt == '#' && *(newIt+1) == '/'){
                notFound = false;
                len++;
                }
            newIt++;
            len++;
        }
        if (notFound){
            return std::make_pair(INVALID, len);
        }
        return std::make_pair(MULTILINECOMMENT, len);
        }
    else if (*it == '#'){
        while (notFound && newIt < end)
        {
            if (*(newIt+1) == '\n')
                notFound = false;
            newIt++;
            len++;
        }
        return std::make_pair(COMMENT, len);
        }
    else if (*it == ',')
        return std::make_pair(COMMA, 1);
    else if (*it == '.')
        return std::make_pair(MEMBEROP, 1);
    else if (*it == ':')
        return std::make_pair(COLON, 1);
    else if (isRelationalOperator(twoChars))
        return std::make_pair(RELATIONALOP, 2);
    else if (islogicalOperator(twoChars))
        return std::make_pair(LOGICALOP, 2);
    else if (isAssignmentOperator(twoChars))
        return std::make_pair(ASSIGNMENTOP, 2);
    else if (isArithmeticOperator(twoChars))
        return std::make_pair(ARITHMETICOP, 2);
    else if (isRelationalOperator(singleChar))
        return std::make_pair(RELATIONALOP, 1);
    else if (islogicalOperator(singleChar))
        return std::make_pair(LOGICALOP, 1);
    else if (isAssignmentOperator(singleChar))
        return std::make_pair(ASSIGNMENTOP, 1);
    else if (isArithmeticOperator(singleChar))
        return std::make_pair(ARITHMETICOP, 1);
    return std::make_pair(INVALID, 1);
}

std::string getType(Token t){
    switch (t.type)
    {
    case STRING:
        return "string";
        break;
    case INTEGER:
        return "int";
        break;
    case FLOAT:
        return "float";
        break;
    case IDENTIFIER:
        return "identifier";
        break;
    case TAB:
        return "tab";
        break;
    case PARENTHESES:
        return "parentheses";
        break;
    case COLON:
        return "colon";
        break;
    case KEYWORD:
        return "keyword";
        break;
    case COMMENT:
        return "Comment";
        break;
    case MULTILINECOMMENT:
        return "multiline comment";
        break;
    case COMMA:
        return "comma";
        break;
    case DATATYPE:
        return "datatype";
        break;
    case ASSIGNMENTOP:
        return "assignment operator";
        break; // Checker function made
    case MEMBEROP:
        return "member operator";
        break;
    case ARITHMETICOP:
        return "arithmetic operator";
        break; // Checker function made
    case RELATIONALOP:
        return "relational operator";
        break; // Checker function made
    case LOGICALOP:
        return "logical operator";
        break; // Checker function made
    case BITWISEOP:
        return "bitwise operator";
        break; // Not implemented for now
    // WHITESPACE
    case SPACE:
        return "space";
        break;
    case WIN_EOL:
        return "windows type EOL";
        break;
    case EOL:
        return "EOL";
        break;
    case EOF_:
        return "EOF";
        break;
    default:
        return "invalid type";
        break;
    }
}

std::string getType(Type t){
    switch (t)
    {
    case STRING:
        return "string";
        break;
    case INTEGER:
        return "int";
        break;
    case FLOAT:
        return "float";
        break;
    case IDENTIFIER:
        return "identifier";
        break;
    case TAB:
        return "tab";
        break;
    case PARENTHESES:
        return "parentheses";
        break;
    case COLON:
        return "colon";
        break;
    case KEYWORD:
        return "keyword";
        break;
    case COMMENT:
        return "Comment";
        break;
    case MULTILINECOMMENT:
        return "multiline comment";
        break;
    case COMMA:
        return "comma";
        break;
    case DATATYPE:
        return "datatype";
        break;
    case ASSIGNMENTOP:
        return "assignment operator";
        break; // Checker function made
    case MEMBEROP:
        return "member operator";
        break;
    case ARITHMETICOP:
        return "arithmetic operator";
        break; // Checker function made
    case RELATIONALOP:
        return "relational operator";
        break; // Checker function made
    case LOGICALOP:
        return "logical operator";
        break; // Checker function made
    case BITWISEOP:
        return "bitwise operator";
        break; // Not implemented for now
    // WHITESPACE
    case SPACE:
        return "space";
        break;
    case WIN_EOL:
        return "windows type EOL";
        break;
    case EOL:
        return "EOL";
        break;
    case EOF_:
        return "EOF";
        break;
    default:
        return "invalid type";
        break;
    }
}