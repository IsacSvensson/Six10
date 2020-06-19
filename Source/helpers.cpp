#include "helpers.hpp"
#include <sstream>
#include <algorithm>

std::string stringWithArrows(std::string text, Position* start, Position* end){
    std::stringstream ss;
    std::string result = "";

    int lineCount;
    ss << text;

    lineCount = end->line - start->line + 1;
    std::string line;
    for(int i = 0; i < start->line; i++)
        std::getline(ss, line);

    for(int i = 0; i < lineCount; i++){
        std::getline(ss, line);
        int colStart, colEnd;
        if (i == 0)
            colStart = start->col;
        else
            colStart = 0;
        if (i == (lineCount - 1))
            colEnd = end->col;
        else
            line.size();
        
        result += "\n\n" + line + '\n';
        result += std::string(colStart, ' ') + std::string(colEnd-colStart, '^') + '\n';
        std::replace(result.begin(), result.end(), '\t', ' ');
    }
    return result;
}