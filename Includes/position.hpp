#ifndef POSITION_HPP
#define POSITION_HPP

#include <string>

class Position{
private:
    int index;
    int line;
    int col;
public:
    Position() : index(0), line(0), col(0) {};
    ~Position() {};
    void advance(char currentChar);
    Position* copy();
};

#endif