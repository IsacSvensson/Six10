#include "error.hpp"
#include "helpers.hpp"
#include "sstream"

std::string Error::toString(){
    std::string toReturn;
    toReturn = errorName + ": '" + details + "'\nFile: \"" + fileName + "\": " + posStart.getPos();
    toReturn += stringWithArrows(posStart.filetext, &posStart, &posEnd);
    return toReturn;
}

std::string RuntimeError::toString(){
    std::string result = "";
    result += generateTraceback();
    result += errorName + ": " + details + "\n";
    result += stringWithArrows(posStart.filetext, &posStart, &posEnd);
    return result;
}

std::string RuntimeError::generateTraceback(){
    std::string result = "";
    std::string temp;
    auto pos = &posStart;
    auto ctx = context;

    std::stringstream ss;

    while (ctx){
        ss << "File " << pos->filename << ", line " << (pos->line + 1) << ", in " << ctx->displayName;
        std::getline(ss, temp);
        result = temp + "\n" +  result;
        pos = ctx->parentEntryPos;
        ctx = ctx->parent;
    }

    return "Traceback (most recent call last):\n" + result;
}