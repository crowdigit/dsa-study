#include <charconv>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <variant>

#include "./expression.h"
#include "./operator.h"

using namespace std;

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
        if (top < op)
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
