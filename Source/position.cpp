#include "position.hpp"
#include <string>
#include <sstream>

void Position::advance(char currentChar){
    index++;
    col++;
    if (currentChar == '\n'){
        line++;
        col = 0;
    }
}

std::string Position::getPos(){
    std::stringstream ss;
    ss << "Line: " << line << ", Col:" << col;

    std::string pos;
    pos = ss.str();
    return pos;
}