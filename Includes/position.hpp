#pragma once
#include <string>

class Position{
public:
    int index;
    int line;
    int col;
    std::string filename;
    std::string filetext;
    Position(std::string fn, std::string ftxt) : index(0), line(0), col(0), filename(fn), filetext(ftxt) {};
    Position(Position &rhs) : index(rhs.index), line(rhs.line), col(rhs.col), filename(rhs.filename), filetext(rhs.filetext) {};
    ~Position() {};
    void advance(char currentChar = ' ');
    std::string getPos();
};
