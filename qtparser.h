#ifndef QTPARSER_H
#define QTPARSER_H
#include <QString>
#include <algorithm>
#include <map>
#include <iostream>
using namespace std;

//https://ru.stackoverflow.com/questions/23842/%D0%9F%D0%B0%D1%80%D1%81%D0%B5%D1%80-%D0%BC%D0%B0%D1%82%D0%B5%D0%BC%D0%B0%D1%82%D0%B8%D1%87%D0%B5%D1%81%D0%BA%D0%B8%D1%85-%D0%B2%D1%8B%D1%80%D0%B0%D0%B6%D0%B5%D0%BD%D0%B8%D0%B9
//адрес парсера
struct Expression {
    Expression(std::string token) : token(token) {}
    Expression(std::string token, Expression a) : token(token), args{ a } {}
    Expression(std::string token, Expression a, Expression b) : token(token), args{ a, b } {}

    std::string token;
    std::vector<Expression> args;
};

class QtParser {
public:
    explicit QtParser(const char* input) : input(input) {}
    Expression parse();
    static double eval(const Expression& e);
private:
    std::string parse_token();
    Expression parse_simple_expression();
    Expression parse_binary_expression(int min_priority);

    const char* input;
};

#endif // QTPARSER_H
