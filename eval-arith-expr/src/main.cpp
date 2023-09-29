#include <iostream>
#include <stack>
#include <string>

#include "./expression.h"

using namespace std;

auto precedence(char const op) -> int
{
    switch (op) {
    case '+':
    case '-':
        return 0;
    case '*':
    case '/':
        return 1;
    case '^':
        return 2;
    }
    return -1;
}

auto processToken(
        stack<string>& op,
        string& result,
        char const c)
{
}

auto evaluate(string const& input) -> int
{
    stack<string> operands;
    string result;

    Expression expr(input);

    return 0;
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
