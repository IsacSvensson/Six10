#include "nodes.hpp"

class Parser{
private:
    std::vector<Token> tokens;
    std::size_t tokIndex = 0;
    Token advance();
    numberNode factor();
};