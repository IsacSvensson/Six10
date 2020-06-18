#include "error.hpp"
#include "helpers.hpp"

std::string Error::toString(){
    std::string toReturn;
    toReturn = errorName + ": '" + details + "'\nFile: \"" + fileName + "\": " + posStart.getPos();
    toReturn += stringWithArrows(posStart.filetext, &posStart, &posEnd);
    return toReturn;
}