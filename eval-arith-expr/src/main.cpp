#include <charconv>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <sstream>
#include <tuple>
#include <variant>

#include "./expression.h"

using namespace std;

enum Operator {
    Plus,
    Minus,
    Multiply,
    Divide,
    Power,
};

auto to_operator(string_view input) -> optional<tuple<Operator, int>>
{
    if (input == "+") return tuple {Plus, 1};
    if (input == "-") return tuple {Minus, 1};
    if (input == "*") return tuple {Multiply, 2};
    if (input == "/") return tuple {Divide, 2};
    if (input == "^") return tuple {Power, 3};
    return nullopt;
}

auto to_char(Operator const op) -> char
{
    switch (op) {
    case Plus:
        return '+';
    case Minus:
        return '-';
    case Multiply:
        return '*';
    case Divide:
        return '/';
    case Power:
        return '^';
    }
}

auto precedence(Operator const op) -> int
{
    switch (op) {
    case Plus:
    case Minus:
        return 1;
    case Multiply:
    case Divide:
        return 2;
    case Power:
        return 3;
    }
    return -1;
}

/*
 * 1+2*3
 * 123*+
 */

auto process_subexpr(
        queue<variant<int, Operator>>& post_expr,
        stack<Operator>& operators,
        string_view subexpr)
{
    auto const result = to_operator(subexpr);
    if (!result.has_value()) {
        int v = 0;
        from_chars(subexpr.data(), subexpr.data() + subexpr.size(), v, 10);
        post_expr.push(v);
        return;
    }

    auto const [op, prec] = result.value();

    while (!operators.empty()) {
        auto const top = operators.top();
        if (precedence(top) < prec)
            break;
        post_expr.push(top);
        operators.pop();
    }

    operators.push(op);
}

auto to_postfix(string const& input) -> queue<variant<int, Operator>>
{
    queue<variant<int, Operator>> post_expr;
    stack<Operator> operators;
    Expression expr(input);

    for (auto const subexpr : expr)
        process_subexpr(post_expr, operators, subexpr);

    while (!operators.empty()) {
        post_expr.push(operators.top());
        operators.pop();
    }

    return post_expr;
}

auto evaluate_sub(Operator const op, int const a, int const b) -> int
{
    switch (op) {
    case Plus: return a + b;
    case Minus: return a - b;
    case Multiply: return a * b;
    case Divide: return a / b;
    case Power: return pow(a, b);
    }
}

auto evaluate(string const& input) -> int
{
    auto postfix = to_postfix(input);
    stack<int> operands;

    while (!postfix.empty()) {
        auto const token = postfix.front(); postfix.pop();
        switch (token.index()) {
        case 0:
            operands.push(get<0>(token));
            break;
        case 1:
            auto const op = get<1>(token);
            auto const b = operands.top(); operands.pop();
            auto const a = operands.top(); operands.pop();
            operands.push(evaluate_sub(op, a, b));
            break;
        }
    }

    return operands.top();
}

auto main(void) -> int
{
    auto const input = "1 + 4 * 2";
    auto const expected = 1 + 4 * 2;
    auto const got = evaluate(input);

    cout << "input: " << input << endl;
    cout << "expected: " << expected << endl;
    cout << "got: " << got << endl;
    if (expected == got)
        cout << "test passed" << endl;
    else
        cout << "test failed" << endl;

    return EXIT_SUCCESS;
}
