#include "./operator.h"

using namespace std;

auto Operator::is_operator(std::string_view input) -> std::optional<Operator>
{
    if (input == "+") return Operator(type::Plus);
    if (input == "-") return Operator(type::Minus);
    if (input == "*") return Operator(type::Multiply);
    if (input == "/") return Operator(type::Divide);
    if (input == "^") return Operator(type::Power);
    return std::nullopt;
}

Operator::Operator(Operator const& o)
    : _type(o._type)
{ }

Operator::Operator(type const type)
    : _type(type)
{ }

Operator::operator char () const {
    switch (_type) {
        case type::Plus: return '+';
        case type::Minus: return '-';
        case type::Multiply: return '*';
        case type::Divide: return '/';
        case type::Power: return '^';
    }
    throw runtime_error("invalid operator");
}


auto Operator::precedence() const -> int
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


auto Operator::evaluate(int const a, int const b) const -> int
{
    switch (_type) {
    case type::Plus: return a + b;
    case type::Minus: return a - b;
    case type::Multiply: return a * b;
    case type::Divide: return a / b;
    case type::Power: return pow(a, b);
    }
}
