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

class Operator {
public:
    enum class type {
        Plus,
        Minus,
        Multiply,
        Divide,
        Power,
    };

    static auto is_operator(string_view input) -> optional<Operator>
    {
        if (input == "+") return Operator(type::Plus);
        if (input == "-") return Operator(type::Minus);
        if (input == "*") return Operator(type::Multiply);
        if (input == "/") return Operator(type::Divide);
        if (input == "^") return Operator(type::Power);
        return nullopt;
    }

    Operator(Operator const& o)
        : _type(o._type)
    { }

    Operator(type const type)
        : _type(type)
    { }

    explicit operator char () const {
        switch (_type) {
            case type::Plus: return '+';
            case type::Minus: return '-';
            case type::Multiply: return '*';
            case type::Divide: return '/';
            case type::Power: return '^';
        }
        throw runtime_error("invalid operator");
    }

    auto precedence() const -> int
    {
        switch (_type) {
            case type::Plus: return 1;
            case type::Minus: return 1;
            case type::Multiply: return 2;
            case type::Divide: return 2;
            case type::Power: return 3;
        }
        throw runtime_error("invalid operator");
    }

    auto evaluate(int const a, int const b) const -> int
    {
        switch (_type) {
        case type::Plus: return a + b;
        case type::Minus: return a - b;
        case type::Multiply: return a * b;
        case type::Divide: return a / b;
        case type::Power: return pow(a, b);
        }
    }

private:
    type _type;
};

auto process_subexpr(
        queue<variant<int, Operator>>& post_expr,
        stack<Operator>& operators,
        string_view subexpr)
{
    auto const result = Operator::is_operator(subexpr);
    if (!result.has_value()) {
        int v = 0;
        from_chars(subexpr.data(), subexpr.data() + subexpr.size(), v, 10);
        post_expr.push(v);
        return;
    }

    auto const op = result.value();

    while (!operators.empty()) {
        auto const top = operators.top();
        if (top.precedence() < op.precedence())
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
            operands.push(op.evaluate(a, b));
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
