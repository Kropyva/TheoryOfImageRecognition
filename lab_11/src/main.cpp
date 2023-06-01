#include <Windows.h>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

using namespace std;

string getString()
{
    string value {};

    try {
        getline(cin, value);
    } catch (const exception& e) {
        cout << "Failed to read a line: " << e.what() << endl;
    }

    return value;
}

class Node {
public:
    virtual int evaluate() const = 0;
};

class NumberNode : public Node {
public:
    NumberNode(int value)
        : value(value)
    {
    }

    int evaluate() const override
    {
        return value;
    }

private:
    int value;
};

class BinaryOpNode : public Node {
public:
    BinaryOpNode(char op, shared_ptr<Node> left, shared_ptr<Node> right)
        : op(op)
        , left(left)
        , right(right)
    {
    }

    int evaluate() const override
    {
        switch (op) {
        case '+':
            return left->evaluate() + right->evaluate();
        case '-':
            return left->evaluate() - right->evaluate();
        case '*':
            return left->evaluate() * right->evaluate();
        case '/':
            return left->evaluate() / right->evaluate();
        default:
            throw runtime_error("Invalid operator");
        }
    }

private:
    char op;
    shared_ptr<Node> left;
    shared_ptr<Node> right;
};

class NodeBuilder {
public:
    explicit NodeBuilder(const string& expression)
        : expression(expression)
        , index(0)
    {
    }

    shared_ptr<Node> buildAST()
    {
        return parseExpression();
    }

private:
    shared_ptr<Node> parseExpression()
    {
        auto left = parseTerm();
        while (match('+') || match('-')) {
            char op = expression[index - 1];
            auto right = parseTerm();
            left = make_shared<BinaryOpNode>(op, move(left), move(right));
        }
        return left;
    }

    shared_ptr<Node> parseTerm()
    {
        auto left = parseFactor();
        while (match('*') || match('/')) {
            char op = expression[index - 1];
            auto right = parseFactor();
            left = make_shared<BinaryOpNode>(op, move(left), move(right));
        }
        return left;
    }

    shared_ptr<Node> parseFactor()
    {
        if (isdigit(peek())) {
            int value = parseNumber();
            return make_shared<NumberNode>(value);
        } else if (match('(')) {
            auto node = parseExpression();
            if (!match(')')) {
                throw runtime_error("Missing closing parenthesis");
            }
            return node;
        } else {
            throw runtime_error("Invalid expression");
        }
    }

    int parseNumber()
    {
        string number;
        while (isdigit(peek())) {
            number += advance();
        }
        return stoi(number);
    }

    char peek() const
    {
        if (index < expression.size()) {
            return expression[index];
        }
        return '\0';
    }

    char advance()
    {
        if (index < expression.size()) {
            return expression[index++];
        }
        return '\0';
    }

    bool match(char expected)
    {
        if (peek() == expected) {
            advance();
            return true;
        }
        return false;
    }

    string expression;
    size_t index;
};

int main()
{
    thread task([]() {
        while (true) {
            this_thread::sleep_for(chrono::milliseconds(100));

            if (GetAsyncKeyState(VK_ESCAPE)) {
                exit(0);
            }
        }
    });

    while (true) {
        // (10+1)*(5+6*(3+1))+4*(3+1)
        cout << "Enter your expression without spaces: " << endl;
        string expression { getString() };
        NodeBuilder builder(expression);

        try {
            shared_ptr<Node> ast = builder.buildAST();
            int result = ast->evaluate();
            cout << "Expression result: " << result << endl;
        } catch (const exception& e) {
            cout << "Failed to create nodes: " << e.what() << endl;
        }
    }

    return 0;
}