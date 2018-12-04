#include <string>
#include <vector>
#include <cmath>
#include <cctype>
#include <cstring>
#include <stdexcept>
#include "qtparser.h"
#include <qDebug>

std::string QtParser::parse_token() {
    while (std::isspace(*input)) ++input;

    if (std::isdigit(*input)) {
        std::string number;
        while (std::isdigit(*input) || *input == '.') number.push_back(*input++);
        return number;
    }

    static const std::string tokens[] =
    { "+", "-", "**", "*", "/", "mod", "abs", "sin", "cos", "(", ")" };
    for (auto& t : tokens) {
        if (std::strncmp(input, t.c_str(), t.size()) == 0) {
            input += t.size();
            return t;
        }
    }

    return "";
}

Expression QtParser::parse_simple_expression() {
    auto token = parse_token();
    if (token.empty()) throw std::runtime_error("Invalid input");

    if (token == "(") {
        auto result = parse();
        if (parse_token() != ")") throw std::runtime_error("Expected ')'");
        return result;
    }

    if (std::isdigit(token[0]))
        return Expression(token);

    return Expression(token, parse_simple_expression());
}

int get_priority(const std::string& binary_op) {
    if (binary_op == "+") return 1;
    if (binary_op == "-") return 1;
    if (binary_op == "*") return 2;
    if (binary_op == "/") return 2;
    if (binary_op == "mod") return 2;
    if (binary_op == "**") return 3;
    return 0;
}

Expression QtParser::parse_binary_expression(int min_priority) {
    auto left_expr = parse_simple_expression();

    for (;;) {
        auto op = parse_token();
        auto priority = get_priority(op);
        if (priority <= min_priority) {
            input -= op.size();
            return left_expr;
        }

        auto right_expr = parse_binary_expression(priority);
        left_expr = Expression(op, left_expr, right_expr);
    }
}

Expression QtParser::parse() {
    return parse_binary_expression(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double QtParser::eval(const Expression& e) {
    switch (e.args.size()) {
    case 2: {
        auto a = eval(e.args[0]);
        auto b = eval(e.args[1]);
        if (e.token == "+") return a + b;
        if (e.token == "-") return a - b;
        if (e.token == "*") return a * b;
        if (e.token == "/") return a / b;
        if (e.token == "**") return pow(a, b);
        if (e.token == "mod") return (int)a % (int)b;
        throw std::runtime_error("Unknown binary operator");
    }

    case 1: {
        auto a = eval(e.args[0]);
        if (e.token == "+") return +a;
        if (e.token == "-") return -a;
        if (e.token == "abs") return abs(a);
        if (e.token == "sin") return sin(a);
        if (e.token == "cos") return cos(a);
        throw std::runtime_error("Unknown unary operator");
    }

    case 0:
        return strtod(e.token.c_str(), nullptr);
    }

    throw std::runtime_error("Unknown expression type");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>

int errors;

void test(const char* input, double expected) {
    try {
        QtParser p(input);
        auto result = QtParser::eval(p.parse());
        if (result == expected) return;
         qDebug() << input << " = " << expected << " : error, got " << result << '\n';
    }
    catch (std::exception& e) {
         qDebug() << input << " : exception: " << e.what() << '\n';
    }
    ++errors;
}
