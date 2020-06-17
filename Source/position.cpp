#include "position.hpp"
#include <string>

void Position::advance(char currentChar){
    index++;
    col++;
    if (currentChar == '\n'){
        line++;
        col = 0;
    }
}

Position* Position::copy(){
    return new Position(*this);
}