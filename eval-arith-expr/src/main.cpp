#include <charconv>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <variant>

#include "./expression.h"
#include "./operator.h"

using namespace std;

auto process_token(
        queue<variant<int, Operator>>& tokens,
        stack<Operator>& operators,
        string_view token)
{
    auto const result = Operator::is_operator(token);
    if (!result.has_value()) {
        int v = 0;
        from_chars(token.data(), token.data() + token.size(), v, 10);
        tokens.push(v);
        return;
    }

    auto const op = result.value();
    if (op.type() == Operator::type::bracket_close) {
        while (!operators.empty()) {
            auto const top = operators.top();
            operators.pop();
            if (top.type() == Operator::type::bracket_open)
                return;
            tokens.push(top);
        }
    }

    while (!operators.empty()) {
        auto const top = operators.top();
        if (top < op)
            break;
        else if (top.type() == Operator::type::bracket_open)
            break;
        tokens.push(top);
        operators.pop();
    }
    operators.push(op);
}

auto to_postfix(string const& input) -> queue<variant<int, Operator>>
{
    queue<variant<int, Operator>> tokens;
    stack<Operator> operators;
    Expression expr(input);

    for (auto const token : expr)
        process_token(tokens, operators, token);

    while (!operators.empty()) {
        tokens.push(operators.top());
        operators.pop();
    }

    return tokens;
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
    auto const input = "3 * ( 1 + 2 ) ^ ( 1 + 2 ) ^ 2";
    auto const expected = 3 * pow(1 + 2, pow(1 + 2, 2));
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
