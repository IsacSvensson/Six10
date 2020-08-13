#include <iostream>
#include <string>
#include <cmath>
#include "test.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

static SymbolTable symtab(10); 

bool testCode(std::string code, double expectedResult){
    Lexer lex(code, "Test module");
    auto tokens = lex.makeTokens();

    if(tokens.second){
        return false;
    }

    Parser p(tokens.first);
    auto res = p.parse();
    if (res->error){
            return false;
    }

    Interpreter interpreter(res->node);
    Context context("<program>");
    context.symTab = &symtab;
    auto result = interpreter.visit(res->node, &context);
    if (result->error){
        if (expectedResult == -9999)
            return true;
        return false;
    }
    if (result->type == INTEGER || result->type == FLOAT)
        return ((Number*)(result->value))->value == expectedResult;
    else if(result->type == FUNCDEF && expectedResult == -9999)
        return true;
    return false;
}

bool testAllFunc(){
    symtab.set("null", new Number(0, INTEGER));
    symtab.set("True", new Number(1, INTEGER));
    symtab.set("False", new Number(0, INTEGER));
    int testNum = 1;
    int successfulTests = 0;
    std::cout << "Test " << testNum << " - Numbers and arithmetic operations:" << std::endl;
    if(testArithm()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Comparison Operations and Logic Operators:" << std::endl;
    if(testCompareAndLogic()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - If-Statments:" << std::endl;
    if(testIfStat()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - For-Loops:" << std::endl;
    if(testForLoop()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - While-Loops:" << std::endl;
    if(testWhileLoop()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Single Line Function:" << std::endl;
    if(testFunction()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Lists:" << std::endl;
    if(testList()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Strings:" << std::endl;
    if(testStrings()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Variables:" << std::endl;
    if(testVariables()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Built-In Functions:" << std::endl;
    if(testBuiltInFunctions()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Return Statments:" << std::endl;
    if(testReturnStat()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Reading files:" << std::endl;
    if(testReadScript()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << '\n' << successfulTests << "/" << testNum-1 << " tests successful!" << std::endl;
    return testNum == successfulTests;
}

bool testArithm(){
    bool finalSuccess = true;
    std::cout << "\tTest 1.1 - Basic Operations ('+', '-', '*', '/' and '^'):" << std::endl;;
    std::string code[5]{"5 + 5", "33 - 10", "3 * 3", "10 / 4", "5 ^ 2"};
    double expVal[5]{10, 23, 9, 2.5, 25};
    bool success = true;
    for (int i = 0; i < 5; i++){
        success = true;
        std::cout << "\t\t" << code[i] << " = " << expVal[i] << ":";
        if (testCode(code[i], expVal[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest 1.1 - Success\n" << std::endl;
    else
        std::cout << "\tTest 1.1 - Fail\n" << std::endl;

    std::cout << "\tTest 1.2 - Multiple Operations:" << std::endl;;
    std::string codeMO[5]{"5 + 5 - 3", "5*9-15/3", "(3 * 3)*3", "(10 / 4 * 4 + 15) ^ 0.5", "(6*6)/(2*2)"};
    double expValMO[5] = {7, 40, 27, 5, 9};
    success = true;
    for (int i = 0; i < 5; i++){
        success = true;
        std::cout << "\t\t" << codeMO[i] << " = " << expValMO[i] << ":";
        if (testCode(codeMO[i], expValMO[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest 1.2 - Success\n" << std::endl;
    else
        std::cout << "\tTest 1.2 - Fail\n" << std::endl;

    std::cout << "\tTest 1.3 - Negative numbers:" << std::endl;;
    std::string codeNN[5]{"-5 + 5", "-3 * 7", "-255 / -5", "-10 ^ 3", "-1"};
    double expValNN[5] = {0, -21, 51, -1000, -1};
    success = true;
    for (int i = 0; i < 5; i++){
        success = true;
        std::cout << "\t\t" << codeNN[i] << " = " << expValNN[i] << ":";
        if (testCode(codeNN[i], expValNN[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest 1.3 - Success\n" << std::endl;
    else
        std::cout << "\tTest 1.3 - Fail\n" << std::endl;

    std::cout << "\tTest 1.4 - Illegal Operations:" << std::endl;;
    std::string codeIO[3]{"5 / 0", "(10 / (5 - 5)) + 10", "0 / 0"};
    double expValIO[3] = {-9999, -9999, -9999};
    success = true;
    for (int i = 0; i < 3; i++){
        success = true;
        std::cout << "\t\t" << codeIO[i] << " = " << "Runtime Error: Division by 0 not allowed" << ":";
        if (testCode(codeIO[i], expValIO[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest 1.4 - Success" << std::endl;
    else
        std::cout << "\tTest 1.4 - Fail" << std::endl;
    return finalSuccess;
}

bool testCompareAndLogic(){
    bool finalSuccess = true;
    std::cout << "\tTest 2.1 - Comparison Operators ('<', '>', '<=', '>=', '==', and '!='):" << std::endl;
    std::string code[18]{"5 < 5", "5 < -1", "5 < 10", "5 > 5", "5 > -1", "5 > 10", "5 <= 5", "5 <= -1", "5 <= 10", 
        "5 >= 5", "5 >= -1", "5 >= 10", "5 == 5", "5 == -1", "5 == 10", "5 != 5", "5 != -1", "5 != 10"};
    double expVal[18]{0,0,1,0,1,0,1,0,1,1,1,0,1,0,0,0,1,1};
    bool success = true;
    for (int i = 0; i < sizeof(code)/sizeof(code[0]); i++){
        success = true;
        std::cout << "\t\t" << code[i] << " = " << expVal[i] << ":";
        if (testCode(code[i], expVal[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest 2.1 - Success\n" << std::endl;
    else
        std::cout << "\tTest 2.1 - Fail\n" << std::endl;
    std::cout << "\tTest 2.2 - Logic Operators ('and'/'&&', 'or'/'||' and 'not'/'!'):" << std::endl;
    std::string codeLogic[16]{"True and False", "True and True", "False and False", "True && False", "True && True", "False && False", "True or False",
        "True or True", "False or False", "True || False", "True || True", "False || False", "not False", "not True", "!False", "!True"};
    double expValLogic[16]{0,1,0,0,1,0,1,1,0,1,1,0,1,0,1,0};
    success = true;
    for (int i = 0; i < 16; i++){
        success = true;
        std::cout << "\t\t" << codeLogic[i] << " = " << expValLogic[i] << ":";
        if (testCode(codeLogic[i], expValLogic[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest 2.2 - Success" << std::endl;
    else
        std::cout << "\tTest 2.2 - Fail" << std::endl;
    return finalSuccess;
}

bool testIfStat(){
    bool finalSuccess = true;
    std::cout << "\tTest 3.1 - True/False:" << std::endl;
    std::string code[4]{"if True then 1 elif False then 2 elif False then 3 else 4", "if False then 1 elif True then 2 elif False then 3 else 4",
        "if False then 1 elif False then 2 elif True then 3 else 4", "if False then 1 elif False then 2 elif False then 3 else 4"};
    double expVal[4]{1,2,3,4};
    bool success = true;
    for (int i = 0; i < 4; i++){
        success = true;
        std::cout << "\t\t" << code[i] << " = " << expVal[i] << ":";
        if (testCode(code[i], expVal[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest 3.1 - Success\n" << std::endl;
    else
        std::cout << "\tTest 3.1 - Fail\n" << std::endl;

    std::cout << "\tTest 3.2 - Eval expr and Nested If Statement:" << std::endl;
    std::string codeExpr[4]{"if 1-1 then 0 else 1", "if 1-2 then 2 else 0",
        "if 0+3 then 3 else 0", "if (if True then 1 else 0) then 4 else 0"};
    double expValExpr[4]{1,2,3,4};
    success = true;
    for (int i = 0; i < 4; i++){
        success = true;
        std::cout << "\t\t" << codeExpr[i] << " = " << expValExpr[i] << ":";
        if (testCode(codeExpr[i], expValExpr[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest 3.2 - Success" << std::endl;
    else
        std::cout << "\tTest 3.2 - Fail" << std::endl;
    return finalSuccess;
}

bool testForLoop(){
    bool finalSuccess = true;
    std::cout << "\tTest 4.1 - Single Line For-Loop:" << std::endl;
    std::cout << "\tCreating variable x = 0\n\n\tProceeding to testing:\n";
    testCode("var x = 0", 0);
    std::string code[2]{"for i = 0 to 6 then var x = x + i", "for i = 0 to 100 step 10 then var x = x + i"};
    double expVal[2]{15, 450};
    bool success = true;
    for (int i = 0; i < 2; i++){
        success = true;
        std::cout << "\t\t" << code[i] << " = " << expVal[i] << ":";
        testCode(code[i], 0);
        if (testCode("x", expVal[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
        std::cout << "\tResetting variable x to 0\n";
        testCode("var x = 0", 0);
    }
    if (success)
        std::cout << "\tTest 4.1 - Success\n" << std::endl;
    else
        std::cout << "\tTest 4.1 - Fail\n" << std::endl;

    /* std::cout << "\tTest 4.2 - Multi Line For-Loops:" << std::endl;
    std::string codeExpr[2]{"for i = 0 to 6 then\n\t\tvar x = x + i \n\t\tvar x = x * i\t\t", "\n\t\tfor i = 0 to 30 step 10 then\n\t\tvar x = x + i \n\t\tvar x = x * x\t\t"};
    double expValExpr[2]{645, 14400};
    success = true;
    for (int i = 0; i < 2; i++){
        success = true;
        std::cout << "\t\t" << codeExpr[i] << " = " << expValExpr[i] << ":";
        if (testCode(codeExpr[i], expValExpr[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest 4.2 - Success" << std::endl;
    else
        std::cout << "\tTest 4.2 - Fail" << std::endl; */
    return finalSuccess;
}

bool testWhileLoop(){
    bool finalSuccess = true;
    std::cout << "\tTest 5.1 - Single Line While Loop:" << std::endl;
    std::string code[3]{"while False then var x = x + 10", "while x < 10 then var x = x + 1", "while not x == 0 then var x = x - 1"};
    double expVal[3]{0, 10, 0};
    bool success = true;
    for (int i = 0; i < sizeof(code)/sizeof(code[0]); i++){
        success = true;
        std::cout << "\t\t" << code[i] << " = " << expVal[i] << ":";
        testCode(code[i], 0);
        if (testCode("x", expVal[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest 5.1 - Success\n" << std::endl;
    else
        std::cout << "\tTest 5.1 - Fail\n" << std::endl;
    return finalSuccess;
}

bool testFunction(){
    bool finalSuccess = true;
    std::cout << "\tTest 6.1 - Single Line Function:" << std::endl;
    std::string code[6]{"def test() -> 100", "def max(a,b) -> if a >= b then a else b", "def sqrt(number) -> number^0.5", "test()", "max(-1, 5)", "sqrt(121)"};
    double expVal[6]{-9999,-9999,-9999, 100, 5, 11};
    bool success = true;

    for (int i = 0; i < sizeof(code)/sizeof(code[0])/2; i++){
        success = true;
        std::cout << "\t\t" << code[i] << ":";
        if (testCode(code[i], expVal[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
        std::cout << "\t\t" << code[i+3] << " = " << expVal[i+3] << ":";
        if (testCode(code[i+3], expVal[i+3]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
        if (i < (sizeof(code)/sizeof(code[0])/2)-1)
            std::cout << std::endl;

    }
    if (success)
        std::cout << "\tTest 6.1 - Success\n" << std::endl;
    else
        std::cout << "\tTest 6.1 - Fail\n" << std::endl;
    return finalSuccess;
}

bool testList(){
    /* bool finalSuccess = true;
    std::cout << "\tTest x.x - xxx:" << std::endl;
    std::string code[]{};
    double expVal[]{};
    bool success = true;
    for (int i = 0; i < sizeof(code)/sizeof(code[0]); i++){
        success = true;
        std::cout << "\t\t" << code[i] << " = " << expVal[i] << ":";
        if (testCode(code[i], expVal[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest x.x - Success\n" << std::endl;
    else
        std::cout << "\tTest x.x - Fail\n" << std::endl; */
    return false;
}

bool testStrings(){
    /* bool finalSuccess = true;
    std::cout << "\tTest x.x - xxx:" << std::endl;
    std::string code[]{};
    double expVal[]{};
    bool success = true;
    for (int i = 0; i < sizeof(code)/sizeof(code[0]); i++){
        success = true;
        std::cout << "\t\t" << code[i] << " = " << expVal[i] << ":";
        if (testCode(code[i], expVal[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest x.x - Success\n" << std::endl;
    else
        std::cout << "\tTest x.x - Fail\n" << std::endl; */
    return false;
}

bool testVariables(){
    /* bool finalSuccess = true;
    std::cout << "\tTest x.x - xxx:" << std::endl;
    std::string code[]{};
    double expVal[]{};
    bool success = true;
    for (int i = 0; i < sizeof(code)/sizeof(code[0]); i++){
        success = true;
        std::cout << "\t\t" << code[i] << " = " << expVal[i] << ":";
        if (testCode(code[i], expVal[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest x.x - Success\n" << std::endl;
    else
        std::cout << "\tTest x.x - Fail\n" << std::endl; */
    return false;
}

bool testBuiltInFunctions(){
    /* bool finalSuccess = true;
    std::cout << "\tTest x.x - xxx:" << std::endl;
    std::string code[]{};
    double expVal[]{};
    bool success = true;
    for (int i = 0; i < sizeof(code)/sizeof(code[0]); i++){
        success = true;
        std::cout << "\t\t" << code[i] << " = " << expVal[i] << ":";
        if (testCode(code[i], expVal[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest x.x - Success\n" << std::endl;
    else
        std::cout << "\tTest x.x - Fail\n" << std::endl; */
    return false;
}

bool testReturnStat(){
    /* bool finalSuccess = true;
    std::cout << "\tTest x.x - xxx:" << std::endl;
    std::string code[]{};
    double expVal[]{};
    bool success = true;
    for (int i = 0; i < sizeof(code)/sizeof(code[0]); i++){
        success = true;
        std::cout << "\t\t" << code[i] << " = " << expVal[i] << ":";
        if (testCode(code[i], expVal[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest x.x - Success\n" << std::endl;
    else
        std::cout << "\tTest x.x - Fail\n" << std::endl; */
    return false;
}

bool testReadScript(){
    /* bool finalSuccess = true;
    std::cout << "\tTest x.x - xxx:" << std::endl;
    std::string code[]{};
    double expVal[]{};
    bool success = true;
    for (int i = 0; i < sizeof(code)/sizeof(code[0]); i++){
        success = true;
        std::cout << "\t\t" << code[i] << " = " << expVal[i] << ":";
        if (testCode(code[i], expVal[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest x.x - Success\n" << std::endl;
    else
        std::cout << "\tTest x.x - Fail\n" << std::endl; */
    return false;
}