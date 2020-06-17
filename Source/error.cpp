#include "error.hpp"

std::string Error::toString(){
    std::string toReturn;
    toReturn = errorName + ": " + details;
    return toReturn;
}